
#include "ShipView.hh"
#include "SlotMessage.hh"
#include "TargetMessage.hh"

namespace bsgo {

ShipView::ShipView(CoordinatorShPtr coordinator,
                   const Repositories &repositories,
                   IMessageQueue *const outputMessageQueue)
  : AbstractView("ship")
  , m_coordinator(std::move(coordinator))
  , m_repositories(repositories)
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

void ShipView::setPlayerShipEntityId(const std::optional<Uuid> ship)
{
  m_playerShipEntityId = ship;
}

bool ShipView::isReady() const noexcept
{
  return m_playerShipEntityId.has_value();
}

bool ShipView::hasTarget() const
{
  return getPlayerTarget().has_value();
}

auto ShipView::getPlayerTarget() const -> std::optional<Entity>
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
      error("Failed to return target name", "Unknown kind " + str(entity.kind->kind()));
      // Not needed because of the error above.
      return {};
  }
}

auto ShipView::getShipsWithin(const IBoundingBox &bbox) const -> std::vector<Entity>
{
  const auto predicate = [](const Entity &entity) {
    if (entity.kind->kind() != EntityKind::SHIP || !entity.exists<StatusComponent>())
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
  const auto slotDbId   = playerShip.weapons[weaponId]->dbId();

  auto message = std::make_unique<SlotMessage>(shipDbId, slotDbId, Slot::WEAPON);
  m_outputMessageQueue->pushMessage(std::move(message));
}

void ShipView::tryActivateSlot(const int slotId) const
{
  const auto playerShip = getPlayerShip();
  const auto shipDbId   = playerShip.dbComp().dbId();
  const auto slotDbId   = playerShip.computers[slotId]->dbId();

  auto message = std::make_unique<SlotMessage>(shipDbId, slotDbId, Slot::COMPUTER);
  m_outputMessageQueue->pushMessage(std::move(message));
}

void ShipView::tryAcquireTarget(const Eigen::Vector3f &position) const
{
  const auto playerShip = getPlayerShip();
  const auto shipDbId   = playerShip.dbComp().dbId();

  std::optional<Uuid> maybeTargetDbId{};
  std::optional<EntityKind> maybeTargetKind{};

  const auto maybeTargetId = m_coordinator->getEntityAt(position);
  if (maybeTargetId)
  {
    const auto target = m_coordinator->getEntity(*maybeTargetId);
    maybeTargetDbId   = target.dbComp().dbId();
    maybeTargetKind   = target.kind->kind();
  }

  m_outputMessageQueue->pushMessage(
    std::make_unique<TargetMessage>(shipDbId, position, maybeTargetKind, maybeTargetDbId));
}

void ShipView::setJumpSystem(const Uuid system)
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
  out.jumpTime    = ship.statusComp().tryGetCurrentJumpTime();

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
  return Status::DEAD == ship.statusComp().status();
}

void ShipView::checkPlayerShipEntityIdExists() const
{
  if (!m_playerShipEntityId)
  {
    error("Expected player ship entity id to exist but it does not");
  }
}

} // namespace bsgo
