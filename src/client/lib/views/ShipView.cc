
#include "ShipView.hh"
#include "SlotMessage.hh"
#include "SystemListMessage.hh"
#include "TargetMessage.hh"

namespace pge {

ShipView::ShipView(bsgo::CoordinatorShPtr coordinator, bsgo::IMessageQueue *const outputMessageQueue)
  : AbstractView("ship")
  , m_coordinator(std::move(coordinator))
  , m_outputMessageQueue(outputMessageQueue)
{
  if (nullptr == m_coordinator)
  {
    throw std::invalid_argument("Expected non null coordinator");
  }
  if (nullptr == m_outputMessageQueue)
  {
    throw std::invalid_argument("Expected non null output message queue");
  }
}

auto ShipView::getPlayerShip() const -> bsgo::Entity
{
  checkPlayerShipEntityIdExists();

  const auto ent = m_coordinator->getEntity(*m_playerShipEntityId);
  if (ent.kind->kind() != bsgo::EntityKind::SHIP)
  {
    error("Expected " + bsgo::str(*m_playerShipEntityId) + " to have kind ship but got "
          + ent.str());
  }
  return ent;
}

void ShipView::setPlayerShipEntityId(const std::optional<bsgo::Uuid> ship)
{
  m_playerShipEntityId = ship;
}

bool ShipView::isReady() const noexcept
{
  return m_playerShipEntityId.has_value() && !m_systems.empty();
}

void ShipView::reset()
{
  m_systems.clear();
  m_playerShipEntityId.reset();
}

bool ShipView::hasTarget() const
{
  return getPlayerTarget().has_value();
}

auto ShipView::getPlayerTarget() const -> std::optional<bsgo::Entity>
{
  const auto ship     = getPlayerShip();
  const auto targetId = ship.targetComp().target();

  if (!targetId)
  {
    return {};
  }

  return m_coordinator->getEntity(*targetId);
}

namespace {
auto determineShipName(const bsgo::Entity &ship, const bsgo::Coordinator &coordinator)
  -> std::string
{
  if (ship.exists<bsgo::OwnerComponent>())
  {
    const auto playerEntityId = ship.ownerComp().owner();
    const auto player         = coordinator.getEntity(playerEntityId);

    return player.nameComp().name();
  }

  return ship.nameComp().name();
}
} // namespace

auto ShipView::getEntityName(const bsgo::Entity &entity) const -> std::string
{
  switch (entity.kind->kind())
  {
    case bsgo::EntityKind::ASTEROID:
      return "asteroid";
    case bsgo::EntityKind::OUTPOST:
      return str(entity.factionComp().faction()) + " outpost";
    case bsgo::EntityKind::SHIP:
      return determineShipName(entity, *m_coordinator);
    default:
      error("Failed to return target name", "Unknown kind " + str(entity.kind->kind()));
      // Not needed because of the error above.
      return {};
  }
}

auto ShipView::getShipsWithin(const bsgo::IBoundingBox &bbox) const -> std::vector<bsgo::Entity>
{
  const auto predicate = [](const bsgo::Entity &entity) {
    if (entity.kind->kind() != bsgo::EntityKind::SHIP || !entity.exists<bsgo::StatusComponent>())
    {
      return false;
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

void ShipView::tryActivateWeapon(const int weaponId) const
{
  const auto playerShip = getPlayerShip();
  const auto shipDbId   = playerShip.dbComp().dbId();

  if (weaponId >= static_cast<int>(playerShip.weapons.size()))
  {
    error("Failed to activate weapon " + std::to_string(weaponId),
          "Ship only has " + std::to_string(playerShip.weapons.size()) + " weapon(s)");
  }

  const auto slotDbId = playerShip.weapons[weaponId]->dbId();

  auto message = std::make_unique<bsgo::SlotMessage>(shipDbId, slotDbId, bsgo::Slot::WEAPON);
  m_outputMessageQueue->pushMessage(std::move(message));
}

void ShipView::tryActivateSlot(const int slotId) const
{
  const auto playerShip = getPlayerShip();
  const auto shipDbId   = playerShip.dbComp().dbId();

  if (slotId >= static_cast<int>(playerShip.computers.size()))
  {
    error("Failed to activate slot " + std::to_string(slotId),
          "Ship only has " + std::to_string(playerShip.computers.size()) + " slotweapon(s)");
  }

  const auto slotDbId = playerShip.computers[slotId]->dbId();

  auto message = std::make_unique<bsgo::SlotMessage>(shipDbId, slotDbId, bsgo::Slot::COMPUTER);
  m_outputMessageQueue->pushMessage(std::move(message));
}

void ShipView::tryAcquireTarget(const Eigen::Vector3f &position) const
{
  const auto playerShip = getPlayerShip();
  const auto shipDbId   = playerShip.dbComp().dbId();

  std::optional<bsgo::Uuid> maybeTargetDbId{};
  std::optional<bsgo::EntityKind> maybeTargetKind{};

  const auto maybeTargetId = m_coordinator->getEntityAt(position);
  if (maybeTargetId)
  {
    const auto target = m_coordinator->getEntity(*maybeTargetId);
    maybeTargetDbId   = target.dbComp().dbId();
    maybeTargetKind   = target.kind->kind();
  }

  m_outputMessageQueue->pushMessage(
    std::make_unique<bsgo::TargetMessage>(shipDbId, position, maybeTargetKind, maybeTargetDbId));
}

void ShipView::setJumpSystem(const bsgo::Uuid system)
{
  m_systemToJumpTo = system;
}

void ShipView::clearJumpSystem()
{
  m_systemToJumpTo.reset();
}

bool ShipView::isJumping() const
{
  const auto ship = getPlayerShip();
  return statusIndicatesJump(ship.statusComp().status());
}

namespace {
auto findSystemName(const std::vector<bsgo::SystemData> &systems, const bsgo::Uuid &systemDbId)
  -> std::optional<std::string>
{
  const auto maybeSystem = std::find_if(systems.begin(),
                                        systems.end(),
                                        [&systemDbId](const bsgo::SystemData &system) {
                                          return system.dbId == systemDbId;
                                        });

  if (maybeSystem != systems.end())
  {
    return maybeSystem->name;
  }

  return {};
}
} // namespace

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

  const auto maybeSystemName = findSystemName(m_systems, *m_systemToJumpTo);
  if (!maybeSystemName)
  {
    error("Failed to find system name for " + bsgo::str(*m_systemToJumpTo));
  }
  out.systemName = *maybeSystemName;

  const auto ship = getPlayerShip();
  // TODO: We should not convert back to time.
  constexpr auto MILLIS_IN_ONE_SECOND = 1000.0f;
  const auto jumpTime                 = core::toMilliseconds(MILLIS_IN_ONE_SECOND
                                             * ship.statusComp().getCurrentJumpTime().toSeconds());
  out.jumpTime                        = jumpTime;

  return out;
}

bool ShipView::isInThreat() const
{
  const auto ship = getPlayerShip();
  return statusIndicatesThreat(ship.statusComp().status());
}

bool ShipView::isDead() const
{
  const auto ship = getPlayerShip();
  return bsgo::Status::DEAD == ship.statusComp().status();
}

void ShipView::handleMessageInternal(const bsgo::IMessage &message)
{
  switch (message.type())
  {
    case bsgo::MessageType::SYSTEM_LIST:
      m_systems = message.as<bsgo::SystemListMessage>().getSystemsData();
      break;
    case bsgo::MessageType::DOCK:
    case bsgo::MessageType::JUMP:
      m_playerShipEntityId.reset();
      break;
    default:
      error("Unsupported message type " + bsgo::str(message.type()));
  }
}

void ShipView::checkPlayerShipEntityIdExists() const
{
  if (!m_playerShipEntityId)
  {
    error("Expected player ship entity id to exist but it does not");
  }
}

} // namespace pge
