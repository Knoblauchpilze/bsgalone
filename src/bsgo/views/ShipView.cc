
#include "ShipView.hh"
#include "DockMessage.hh"
#include "EquipMessage.hh"
#include "JumpCancelledMessage.hh"
#include "JumpRequestedMessage.hh"
#include "LockerUtils.hh"
#include "SlotMessage.hh"
#include "TargetMessage.hh"
#include "VelocityMessage.hh"

namespace bsgo {

ShipView::ShipView(const CoordinatorShPtr &coordinator,
                   const Repositories &repositories,
                   IMessageQueue *const messageQueue)
  : AbstractView("ship", coordinator, repositories, messageQueue)
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

auto ShipView::getPlayerShipDbId() const -> Uuid
{
  checkPlayerShipDbIdExists();
  return *m_playerShipDbId;
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

auto ShipView::getWeaponsCount() const -> int
{
  return getPlayerShip().weapons.size();
}

auto ShipView::getAbilitiesCount() const -> int
{
  return getPlayerShip().computers.size();
}

void ShipView::tryActivateWeapon(const Uuid &ship, const int weaponId) const
{
  const auto playerShip = getPlayerShip();
  if (ship != playerShip.uuid)
  {
    error("Failed to activate weapon " + std::to_string(weaponId),
          "Expected ship " + str(playerShip.uuid) + " but got " + str(ship));
  }

  auto message = std::make_unique<SlotMessage>(ship, weaponId, Slot::WEAPON);
  m_messageQueue->pushMessage(std::move(message));
}

void ShipView::tryActivateSlot(const Uuid &ship, const int slotId) const
{
  const auto playerShip = getPlayerShip();
  if (ship != playerShip.uuid)
  {
    error("Failed to activate slot " + std::to_string(slotId),
          "Expected ship " + str(playerShip.uuid) + " but got " + str(ship));
  }

  auto message = std::make_unique<SlotMessage>(ship, slotId, Slot::COMPUTER);
  m_messageQueue->pushMessage(std::move(message));
}

void ShipView::dockPlayerShip() const
{
  auto playerShip = getPlayerShip();
  checkPlayerShipDbIdExists();

  auto message = std::make_unique<DockMessage>(*m_playerShipDbId, playerShip.uuid, true);
  m_messageQueue->pushMessage(std::move(message));
}

void ShipView::undockPlayerShip() const
{
  auto playerShip = getPlayerShip();
  checkPlayerShipDbIdExists();

  auto message = std::make_unique<DockMessage>(*m_playerShipDbId, playerShip.uuid, false);
  m_messageQueue->pushMessage(std::move(message));
}

void ShipView::setJumpSystem(const Uuid &system)
{
  m_systemToJumpTo = system;
}

void ShipView::clearJumpSystem()
{
  m_systemToJumpTo.reset();
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

  checkPlayerShipDbIdExists();

  auto message = std::make_unique<JumpRequestedMessage>(*m_playerShipDbId,
                                                        *m_playerShipEntityId,
                                                        *m_systemToJumpTo);
  m_messageQueue->pushMessage(std::move(message));
}

void ShipView::cancelJump() const
{
  if (!isJumping())
  {
    return;
  }

  checkPlayerShipDbIdExists();

  auto message = std::make_unique<JumpCancelledMessage>(*m_playerShipDbId, *m_playerShipEntityId);
  m_messageQueue->pushMessage(std::move(message));
}

void ShipView::accelerateShip(const Uuid &ship, const Eigen::Vector3f &acceleration) const
{
  m_messageQueue->pushMessage(std::make_unique<VelocityMessage>(ship, acceleration));
}

void ShipView::tryAcquireTarget(const Uuid &ship, const Eigen::Vector3f &position) const
{
  m_messageQueue->pushMessage(std::make_unique<TargetMessage>(ship, position));
}

void ShipView::tryEquipItem(const Item &itemType, const bsgo::Uuid &itemDbId) const
{
  checkPlayerShipDbIdExists();
  m_messageQueue->pushMessage(
    std::make_unique<EquipMessage>(bsgo::EquipType::EQUIP, *m_playerShipDbId, itemType, itemDbId));
}

void ShipView::tryUnequipItem(const Item &itemType, const bsgo::Uuid &itemDbId) const
{
  checkPlayerShipDbIdExists();
  m_messageQueue->pushMessage(
    std::make_unique<EquipMessage>(bsgo::EquipType::UNEQUIP, *m_playerShipDbId, itemType, itemDbId));
}

auto ShipView::getPlayerShipWeapons() const -> std::vector<PlayerWeapon>
{
  checkPlayerShipDbIdExists();

  const auto weapons = m_repositories.shipWeaponRepository->findAllByShip(*m_playerShipDbId);

  std::vector<PlayerWeapon> out;
  for (const auto &weapon : weapons)
  {
    const auto data = m_repositories.playerWeaponRepository->findOneById(weapon.weapon);
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

bool ShipView::canStillEquipItem(const Item &type) const
{
  checkPlayerShipDbIdExists();

  bool equipable{false};
  if (Item::WEAPON == type)
  {
    EquipData data{.shipId           = *m_playerShipDbId,
                   .shipWeaponRepo   = m_repositories.shipWeaponRepository,
                   .shipComputerRepo = m_repositories.shipComputerRepository,
                   .playerShipRepo   = m_repositories.playerShipRepository};
    equipable = canStillEquipWeapon(data);
  }
  if (Item::COMPUTER == type)
  {
    EquipData data{.shipId           = *m_playerShipDbId,
                   .shipWeaponRepo   = m_repositories.shipWeaponRepository,
                   .shipComputerRepo = m_repositories.shipComputerRepository,
                   .playerShipRepo   = m_repositories.playerShipRepository};
    equipable = canStillEquipComputer(data);
  }

  return equipable;
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

bool ShipView::isInThreat() const
{
  const auto ship = getPlayerShip();
  return bsgo::statusIndicatesThreat(ship.statusComp().status());
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
