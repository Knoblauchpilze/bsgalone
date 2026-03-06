
#include "ShipView.hh"
#include "SlotMessage.hh"
#include "SystemListMessage.hh"
#include "TargetPickedMessage.hh"

namespace bsgalone::client {

ShipView::ShipView(GameSessionShPtr gameSession,
                   core::CoordinatorShPtr coordinator,
                   core::IMessageQueue *const outputMessageQueue)
  : AbstractView("ship",
                 {
                   core::MessageType::DOCK,
                   core::MessageType::JUMP,
                   core::MessageType::SYSTEM_LIST,
                 })
  , m_gameSession(std::move(gameSession))
  , m_coordinator(std::move(coordinator))
  , m_outputMessageQueue(outputMessageQueue)
{
  if (nullptr == m_gameSession)
  {
    throw std::invalid_argument("Expected non null game session");
  }
  if (nullptr == m_coordinator)
  {
    throw std::invalid_argument("Expected non null coordinator");
  }
  if (nullptr == m_outputMessageQueue)
  {
    throw std::invalid_argument("Expected non null output message queue");
  }
}

auto ShipView::getPlayerShip() const -> core::Entity
{
  checkPlayerShipEntityIdExists();

  const auto ent = m_coordinator->getEntity(*m_playerShipEntityId);
  if (ent.kind->kind() != core::EntityKind::SHIP)
  {
    error("Expected " + core::str(*m_playerShipEntityId) + " to have kind ship but got "
          + ent.str());
  }
  return ent;
}

void ShipView::setPlayerShipEntityId(const std::optional<core::Uuid> ship)
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

auto ShipView::gameSession() const -> const GameSession &
{
  return *m_gameSession;
}

bool ShipView::hasTarget() const
{
  return getPlayerTarget().has_value();
}

auto ShipView::getPlayerTarget() const -> std::optional<core::Entity>
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
auto determineShipName(const core::Entity &ship, const core::Coordinator &coordinator)
  -> std::string
{
  if (ship.exists<core::OwnerComponent>())
  {
    const auto playerEntityId = ship.ownerComp().owner();
    const auto player         = coordinator.getEntity(playerEntityId);

    return player.nameComp().name();
  }

  return ship.nameComp().name();
}
} // namespace

auto ShipView::getEntityName(const core::Entity &entity) const -> std::string
{
  switch (entity.kind->kind())
  {
    case core::EntityKind::ASTEROID:
      return "asteroid";
    case core::EntityKind::OUTPOST:
      return str(entity.factionComp().faction()) + " outpost";
    case core::EntityKind::SHIP:
      return determineShipName(entity, *m_coordinator);
    default:
      error("Failed to return target name", "Unknown kind " + str(entity.kind->kind()));
      // Not needed because of the error above.
      return {};
  }
}

auto ShipView::getShipsWithin(const core::IBoundingBox &bbox) const -> std::vector<core::Entity>
{
  const auto predicate = [](const core::Entity &entity) {
    if (entity.kind->kind() != core::EntityKind::SHIP || !entity.exists<core::StatusComponent>())
    {
      return false;
    }
    return statusAllowsInteraction(entity.statusComp().status());
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

  const auto slotDbId   = playerShip.weapons[weaponId]->dbId();
  const auto playerDbId = m_gameSession->getPlayerDbId();
  const auto systemDbId = m_gameSession->getSystemDbId();

  auto message = std::make_unique<core::SlotMessage>(playerDbId,
                                                     systemDbId,
                                                     shipDbId,
                                                     slotDbId,
                                                     core::Slot::WEAPON);
  m_outputMessageQueue->pushEvent(std::move(message));
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

  const auto slotDbId   = playerShip.computers[slotId]->dbId();
  const auto playerDbId = m_gameSession->getPlayerDbId();
  const auto systemDbId = m_gameSession->getSystemDbId();

  auto message = std::make_unique<core::SlotMessage>(playerDbId,
                                                     systemDbId,
                                                     shipDbId,
                                                     slotDbId,
                                                     core::Slot::COMPUTER);
  m_outputMessageQueue->pushEvent(std::move(message));
}

void ShipView::tryAcquireTarget(const Eigen::Vector3f &position) const
{
  const auto playerShip = getPlayerShip();

  core::Target data{
    .sourceDbId = playerShip.dbComp().dbId(),
    .sourceKind = core::EntityKind::SHIP,
  };

  const auto maybeTargetId = m_coordinator->getEntityAt(position, {}, core::EntityKind::BULLET);
  if (maybeTargetId)
  {
    const auto target = m_coordinator->getEntity(*maybeTargetId);
    data.targetDbId   = target.dbComp().dbId();
    data.targetKind   = target.kind->kind();
  }

  const auto playerDbId = m_gameSession->getPlayerDbId();
  const auto systemDbId = m_gameSession->getSystemDbId();

  m_outputMessageQueue->pushEvent(
    std::make_unique<core::TargetPickedMessage>(playerDbId, systemDbId, data, position));
}

void ShipView::setJumpSystem(const core::Uuid system)
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
auto findSystemName(const std::vector<core::SystemData> &systems, const core::Uuid &systemDbId)
  -> std::optional<std::string>
{
  const auto maybeSystem = std::find_if(systems.begin(),
                                        systems.end(),
                                        [&systemDbId](const core::SystemData &system) {
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

  const auto maybeSystemName = findSystemName(m_systems, *m_systemToJumpTo);
  if (!maybeSystemName)
  {
    error("Failed to find system name for " + core::str(*m_systemToJumpTo));
  }

  const auto ship = getPlayerShip();

  return JumpData{
    .systemName = *maybeSystemName,
    .jumpTime   = ship.statusComp().getCurrentJumpTime(),
  };
}

bool ShipView::isInThreat() const
{
  const auto ship = getPlayerShip();
  return statusIndicatesThreat(ship.statusComp().status());
}

bool ShipView::isDead() const
{
  const auto ship = getPlayerShip();
  return core::Status::DEAD == ship.statusComp().status();
}

void ShipView::handleMessageInternal(const core::IMessage &message)
{
  switch (message.type())
  {
    case core::MessageType::SYSTEM_LIST:
      m_systems = message.as<core::SystemListMessage>().getSystemsData();
      break;
    case core::MessageType::DOCK:
    case core::MessageType::JUMP:
      m_playerShipEntityId.reset();
      break;
    default:
      error("Unsupported message type " + str(message.type()));
  }
}

void ShipView::checkPlayerShipEntityIdExists() const
{
  if (!m_playerShipEntityId)
  {
    error("Expected player ship entity id to exist but it does not");
  }
}

} // namespace bsgalone::client
