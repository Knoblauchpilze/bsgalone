
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

auto ShipView::getPlayerShipName() const -> std::string
{
  checkPlayerShipDbIdExists();

  const auto ship = m_repositories.playerShipRepository->findOneById(*m_playerShipDbId);
  return ship.name;
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

auto ShipView::getPlayerTargetName() const -> std::optional<std::string>
{
  checkPlayerShipEntityIdExists();

  const auto ent      = m_coordinator->getEntity(*m_playerShipEntityId);
  const auto targetId = ent.targetComp().target();
  if (!targetId)
  {
    return {};
  }

  const auto target     = m_coordinator->getEntity(*targetId);
  const auto targetKind = target.kind->kind();
  if (EntityKind::ASTEROID == targetKind)
  {
    return "asteroid";
  }
  if (EntityKind::OUTPOST == targetKind)
  {
    const auto faction = target.factionComp().faction();
    return str(faction) + " outpost";
  }
  if (EntityKind::SHIP == targetKind)
  {
    const auto faction = target.factionComp().faction();
    return str(faction) + " ship";
  }

  error("Failed to return target name", "Unknown kind " + bsgo::str(targetKind));
  // Not needed because of the error above.
  return {};
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

void ShipView::startJump() const
{
  auto playerShip   = getPlayerShip();
  const auto status = playerShip.statusComp().status();

  if (bsgo::statusIndicatesJump(status))
  {
    return;
  }

  log("Starting jump");
  const auto newStatus = updateStatusForJump(status);
  playerShip.statusComp().setStatus(newStatus);
}

void ShipView::cancelJump() const
{
  auto playerShip   = getPlayerShip();
  const auto status = playerShip.statusComp().status();

  if (!bsgo::statusIndicatesJump(status))
  {
    return;
  }

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
