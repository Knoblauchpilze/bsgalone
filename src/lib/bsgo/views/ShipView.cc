
#include "ShipView.hh"

namespace bsgo {

ShipView::ShipView(const CoordinatorShPtr &coordinator, const Repositories &repositories)
  : IView("ship", coordinator, repositories)
{}

void ShipView::setPlayerShipDbId(const Uuid &ship)
{
  m_playerShipDbId = ship;
}

void ShipView::setPlayerShipEntityId(const Uuid &ship)
{
  m_playerShipEntityId = ship;
}

bool ShipView::isReady() const noexcept
{
  return m_playerShipDbId && m_playerShipEntityId;
}

auto ShipView::getPlayerShip() const -> Entity
{
  checkPlayerShipEntityIdExists();

  const auto ent = m_coordinator->getEntity(*m_playerShipEntityId);
  if (ent.kind->kind() != EntityKind::SHIP)
  {
    error("Expected " + str(*m_playerShipEntityId) + " to have kind ship but got " + ent.str());
  }
  return ent;
}

bool ShipView::hasTarget() const
{
  return getPlayerTarget().has_value();
}

auto ShipView::getPlayerTarget() const -> std::optional<Entity>
{
  checkPlayerShipEntityIdExists();

  const auto ent      = m_coordinator->getEntity(*m_playerShipEntityId);
  const auto targetId = ent.targetComp().target();
  if (!targetId)
  {
    return {};
  }

  return {m_coordinator->getEntity(*targetId)};
}

namespace {
auto determineShipName(const Entity &ship, const Coordinator &coordinator) -> std::string
{
  if (ship.exists<OwnerComponent>())
  {
    const auto playerEntityId = ship.ownerComp().owner();
    const auto player         = coordinator.getEntity(playerEntityId);

    return player.nameComp().name();
  }

  return ship.nameComp().name();
}
} // namespace

auto ShipView::getEntityName(const Entity &entity) const -> std::string
{
  switch (entity.kind->kind())
  {
    case EntityKind::ASTEROID:
      return "asteroid";
    case EntityKind::OUTPOST:
      return str(entity.factionComp().faction()) + " outpost";
    case EntityKind::SHIP:
      return determineShipName(entity, *m_coordinator);
    default:
      error("Failed to return target name", "Unknown kind " + bsgo::str(entity.kind->kind()));
      // Not needed because of the error above.
      return {};
  }
}

auto ShipView::getShipsWithin(const IBoundingBox &bbox) const -> std::vector<Entity>
{
  const auto predicate = [](const Entity &entity) {
    if (entity.kind->kind() != EntityKind::SHIP)
    {
      return false;
    }
    if (!entity.exists<StatusComponent>())
    {
      return true;
    }
    return statusAllowsInteratction(entity.statusComp().status());
  };

  return m_coordinator->getEntitiesWithinSatistying(bbox, predicate);
}

auto ShipView::distanceToTarget() const -> float
{
  const auto target = getPlayerTarget();
  if (!target)
  {
    error("Can't get distance to non existing target");
  }

  const auto playerShip = getPlayerShip();
  const auto playerPos  = playerShip.transformComp().position();
  const auto targetPos  = target->transformComp().position();

  return (targetPos - playerPos).norm();
}

auto ShipView::getAbilitiesCount() const -> int
{
  return getPlayerShip().computers.size();
}

void ShipView::tryActivateWeapon(const Uuid &ship, const int weaponId) const
{
  checkPlayerShipEntityIdExists();
  if (ship != *m_playerShipEntityId)
  {
    error("Failed to activate weapon " + std::to_string(weaponId),
          "Expected ship " + str(*m_playerShipEntityId) + " but got " + str(ship));
  }

  auto data = getPlayerShip();
  if (data.weapons.size() < static_cast<std::size_t>(weaponId))
  {
    error("Failed to activate weapon " + std::to_string(weaponId),
          "Ship only has " + std::to_string(data.weapons.size()) + " weapon(s)");
  }

  const auto weapon = data.weapons[weaponId];
  weapon->toggle();
}

void ShipView::tryActivateSlot(const Uuid &ship, const int slotId) const
{
  checkPlayerShipEntityIdExists();
  if (ship != *m_playerShipEntityId)
  {
    error("Failed to activate slot " + std::to_string(slotId),
          "Expected ship " + str(*m_playerShipEntityId) + " but got " + str(ship));
  }

  auto data = getPlayerShip();
  if (data.computers.size() < static_cast<std::size_t>(slotId))
  {
    error("Failed to activate slot " + std::to_string(slotId),
          "Ship only has " + std::to_string(data.computers.size()) + " computer(s)");
  }

  const auto computer = data.computers[slotId];
  computer->registerFireRequest();
}

void ShipView::dockPlayerShip() const
{
  auto playerShip = getPlayerShip();
  playerShip.statusComp().setStatus(Status::DOCKED);
}

void ShipView::undockPlayerShip() const
{
  auto playerShip = getPlayerShip();
  playerShip.statusComp().setStatus(Status::APPEARING);
}

void ShipView::setJumpSystem(const Uuid &system)
{
  m_systemToJumpTo = system;
}

void ShipView::startJump() const
{
  if (isJumping())
  {
    return;
  }
  if (!m_systemToJumpTo)
  {
    return;
  }

  auto playerShip   = getPlayerShip();
  const auto status = playerShip.statusComp().status();

  log("Starting jump to " + str(*m_systemToJumpTo));
  const auto newStatus = updateStatusForJump(status);
  playerShip.statusComp().setStatus(newStatus);
}

void ShipView::cancelJump() const
{
  if (!isJumping())
  {
    return;
  }

  auto playerShip   = getPlayerShip();
  const auto status = playerShip.statusComp().status();

  log("Cancelling jump");
  const auto newStatus = updateStatusAfterJumpCancellation(status);
  playerShip.statusComp().setStatus(newStatus);
}

auto ShipView::getPlayerShipWeapons() const -> std::vector<PlayerWeapon>
{
  checkPlayerShipDbIdExists();

  const auto weapons = m_repositories.shipWeaponRepository->findAllByShip(*m_playerShipDbId);

  std::vector<PlayerWeapon> out;
  for (const auto &weapon : weapons)
  {
    const auto data = m_repositories.playerWeaponRepository->findOneById(weapon.id);
    out.push_back(data);
  }
  return out;
}

auto ShipView::getPlayerShipComputers() const -> std::vector<PlayerComputer>
{
  checkPlayerShipDbIdExists();

  const auto ids = m_repositories.shipComputerRepository->findAllByShip(*m_playerShipDbId);

  std::vector<PlayerComputer> out;
  for (const auto &id : ids)
  {
    const auto computer = m_repositories.playerComputerRepository->findOneById(id);
    out.push_back(computer);
  }
  return out;
}

auto ShipView::getPlayerShipSlots() const -> std::unordered_map<Slot, int>
{
  checkPlayerShipDbIdExists();

  const auto ship = m_repositories.playerShipRepository->findOneById(*m_playerShipDbId);
  return ship.slots;
}

bool ShipView::isJumping() const
{
  const auto ship = getPlayerShip();
  return bsgo::statusIndicatesJump(ship.statusComp().status());
}

auto ShipView::getJumpData() const -> JumpData
{
  if (!isJumping())
  {
    error("Expected jump to be started but it is not");
  }
  if (!m_systemToJumpTo)
  {
    error("Expected to have a system to jump to");
  }

  JumpData out{};

  const auto system = m_repositories.systemRepository->findOneById(*m_systemToJumpTo);
  out.systemName    = system.name;

  const auto ship = getPlayerShip();
  out.jumpTime    = ship.statusComp().tryGetRemainingJumpTime();

  return out;
}

void ShipView::checkPlayerShipDbIdExists() const
{
  if (!m_playerShipDbId)
  {
    error("Expected player ship db id to exist but it does not");
  }
}

void ShipView::checkPlayerShipEntityIdExists() const
{
  if (!m_playerShipEntityId)
  {
    error("Expected player ship entity id to exist but it does not");
  }
}

} // namespace bsgo
