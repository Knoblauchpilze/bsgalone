
#include "ShipService.hh"
#include "ShipDataSource.hh"

namespace bsgo {

ShipService::ShipService(const Repositories &repositories,
                         CoordinatorShPtr coordinator,
                         DatabaseEntityMapper &entityMapper)
  : AbstractService("ship", repositories)
  , m_coordinator(std::move(coordinator))
  , m_entityMapper(entityMapper)
{}

bool ShipService::trySelectShip(const Uuid shipDbId) const
{
  const auto newActiveShip     = m_repositories.playerShipRepository->findOneById(shipDbId);
  const auto currentActiveShip = m_repositories.playerShipRepository->findOneByPlayerAndActive(
    newActiveShip.player);

  if (newActiveShip.active)
  {
    warn("Failed to select ship " + str(shipDbId), "Ship is already active");
    return false;
  }
  if (currentActiveShip.player != newActiveShip.player)
  {
    warn("Failed to select ship " + str(shipDbId),
         "Current ship belongs to " + str(currentActiveShip.player) + " but new one belongs to "
           + str(newActiveShip.player));
    return false;
  }

  switchActiveShip(currentActiveShip, newActiveShip);
  switchShipSystem(currentActiveShip, newActiveShip);

  return true;
}

bool ShipService::tryDock(const Uuid shipDbId) const
{
  const auto maybeEntityId = m_entityMapper.tryGetShipEntityId(shipDbId);
  if (!maybeEntityId)
  {
    warn("Failed to dock ship " + str(shipDbId), "No entity attached to it");
    return false;
  }

  auto shipEntity  = m_coordinator->getEntity(*maybeEntityId);
  auto &statusComp = shipEntity.statusComp();

  if (!statusAllowsDocking(statusComp.status()))
  {
    warn("Failed to dock ship " + str(shipDbId),
         "Status " + str(statusComp.status()) + " does not allow docking");
    return false;
  }

  const auto ship = m_repositories.playerShipRepository->findOneById(shipDbId);
  m_repositories.systemRepository->updateSystemForShip(shipDbId, *ship.system, true);

  statusComp.setStatus(Status::DOCKED);

  return true;
}

bool ShipService::tryReturnToOutpost(const Uuid shipDbId) const
{
  const auto ship = m_repositories.playerShipRepository->findOneById(shipDbId);
  if (!ship.docked)
  {
    warn("Failed to send ship ship " + str(shipDbId) + " to outpost", "Ship is not already docked");
    return false;
  }

  return true;
}

bool ShipService::accelerateShip(const Uuid shipDbId, const Eigen::Vector3f &acceleration) const
{
  const auto maybeEntityId = m_entityMapper.tryGetShipEntityId(shipDbId);
  if (!maybeEntityId)
  {
    warn("Failed to dock ship " + str(shipDbId), "No entity attached to it");
    return {};
  }

  auto ship = m_coordinator->getEntity(*maybeEntityId);

  if (!ship.exists<OwnerComponent>())
  {
    // Ship is an AI: we should not come through this route to change its
    // acceleration but rather through the AI system directly.
    warn("Failed to accelerate ship " + str(shipDbId), "Ship does not belong to a player");
    return false;
  }

  ship.velocityComp().accelerate(acceleration);

  if (ship.exists<NetworkSyncComponent>())
  {
    ship.networkSyncComp().markForSync();
  }

  return true;
}

auto ShipService::getPlayerDbIdForShip(const Uuid shipDbId) -> Uuid
{
  const auto ship = m_repositories.playerShipRepository->findOneById(shipDbId);
  return ship.player;
}

auto ShipService::getSystemDbIdForShip(const Uuid shipDbId) const -> Uuid
{
  const auto ship = m_repositories.playerShipRepository->findOneById(shipDbId);
  if (!ship.system)
  {
    error("Expected ship " + str(shipDbId) + " to be present in a system");
  }

  return *ship.system;
}

namespace {
auto tryGetEntityAt(const Coordinator &coordinator, const Eigen::Vector3f &position)
  -> std::optional<Entity>
{
  auto maybeEntityId = coordinator.getEntityAt(position, {}, bsgalone::core::EntityKind::BULLET);
  if (!maybeEntityId)
  {
    return {};
  }

  const auto entity = coordinator.getEntity(*maybeEntityId);
  if (entity.exists<StatusComponent>() && !statusVisibleFromDradis(entity.statusComp().status()))
  {
    return {};
  }

  return entity;
}

auto tryGetEntityFromHint(const Uuid entityDbId,
                          const bsgalone::core::EntityKind entityKind,
                          const DatabaseEntityMapper &entityMapper,
                          const Coordinator &coordinator) -> std::optional<Entity>
{
  const auto entityId = entityMapper.tryGetEntityId(entityDbId, entityKind);

  if (!entityId)
  {
    return {};
  }

  return coordinator.getEntity(*entityId);
}

constexpr auto MAXIMUM_TARGET_DISTANCE_TOLERANCE = 1.0f;

auto overrideTargetWithHintIfNecessary(const std::optional<Entity> &maybeTarget,
                                       const std::optional<Entity> &maybeHint,
                                       const Eigen::Vector3f &referencePosition)
  -> std::optional<Entity>
{
  auto dToTarget = std::numeric_limits<float>::max();
  if (maybeTarget)
  {
    dToTarget = (maybeTarget->transformComp().position() - referencePosition).norm();
  }

  auto dToHint = std::numeric_limits<float>::max();
  if (maybeHint)
  {
    dToHint = (maybeHint->transformComp().position() - referencePosition).norm();
  }

  if (dToTarget <= dToHint)
  {
    return maybeTarget;
  }

  // Only consider the hint if it is not too far from the reference position.
  if (dToHint <= MAXIMUM_TARGET_DISTANCE_TOLERANCE)
  {
    return maybeHint;
  }

  return {};
}
} // namespace

auto ShipService::tryAcquireTarget(const TargetAcquiringData &data) const -> AcquiringResult
{
  if (data.sourceKind != bsgalone::core::EntityKind::SHIP)
  {
    error("Unsupported entity acquiring target: " + str(data.sourceKind));
  }

  const auto maybeEntityId = m_entityMapper.tryGetShipEntityId(data.sourceDbId);
  if (!maybeEntityId)
  {
    warn("Failed to acquire target for " + str(data.sourceKind) + " " + str(data.sourceDbId),
         "No entity attached to it");
    return {};
  }

  auto ship = m_coordinator->getEntity(*maybeEntityId);

  auto maybeTarget = tryGetEntityAt(*m_coordinator, data.position);
  if (data.targetDbIdHint && data.targetKindHint)
  {
    const auto maybeHint = tryGetEntityFromHint(*data.targetDbIdHint,
                                                *data.targetKindHint,
                                                m_entityMapper,
                                                *m_coordinator);

    const Eigen::Vector3f shipPosition = ship.transformComp().position();

    std::optional<Uuid> oldTarget{};
    if (maybeTarget)
    {
      oldTarget = maybeTarget->uuid;
    }

    maybeTarget = overrideTargetWithHintIfNecessary(maybeTarget, maybeHint, shipPosition);
    if (maybeTarget && oldTarget && maybeTarget->uuid != *oldTarget)
    {
      warn("Picked hint target " + maybeHint->str() + " over actual target");
    }
  }

  // Self selection is not allowed, return an empty target if it appears that an entity
  // is trying to target itself.
  if (maybeTarget && isSelfSelection(data, *maybeTarget))
  {
    // We return success with no target to indicate that the operation did not fail but
    // was blocked by game logic.
    return AcquiringResult{.success = true};
  }

  std::optional<Uuid> maybeTargetDbId{};
  std::optional<bsgalone::core::EntityKind> maybeTargetKind{};
  if (maybeTarget)
  {
    maybeTargetKind = maybeTarget->kind->kind();
    maybeTargetDbId = maybeTarget->dbComp().dbId();
  }

  std::optional<Uuid> maybeTargetId{};
  if (maybeTarget)
  {
    maybeTargetId = maybeTarget->uuid;
  }
  updateEntityTarget(ship, maybeTargetId);

  return AcquiringResult{.success    = true,
                         .targetKind = maybeTargetKind,
                         .targetDbId = maybeTargetDbId};
}

void ShipService::switchActiveShip(PlayerShip currentActiveShip, PlayerShip newActiveShip) const
{
  currentActiveShip.active = false;
  newActiveShip.active     = true;

  m_repositories.playerShipRepository->save(currentActiveShip);
  m_repositories.playerShipRepository->save(newActiveShip);
}

void ShipService::switchShipSystem(const PlayerShip &currentActiveShip,
                                   const PlayerShip &newActiveShip) const
{
  m_repositories.systemRepository->updateShipForSystem(currentActiveShip.id, newActiveShip.id);
}

void ShipService::updateEntityTarget(Entity &entity, const std::optional<Uuid> &targetId) const
{
  if (!entity.exists<TargetComponent>())
  {
    return;
  }

  auto &targetComp = entity.targetComp();
  if (!targetId)
  {
    targetComp.clearTarget();
    return;
  }

  const auto target = m_coordinator->getEntity(*targetId);
  info("Found target " + target.str());

  targetComp.setTarget(*targetId);
}

bool ShipService::isSelfSelection(const TargetAcquiringData &data, const Entity &target) const
{
  if (data.sourceKind != bsgalone::core::EntityKind::SHIP)
  {
    error("Unsupported entity with kind " + str(data.sourceKind) + " for target selection");
  }

  // In case the target entity does not have a DB component, it means it can't be a ship
  // belonging to the same player and therefore it's not a self selection.
  if (!target.exists<DbComponent>())
  {
    return false;
  }

  // Currently only ships can select targets. If the target is not a ship it means that
  // it can't possibly belong to the same player as only ships are attached to players
  // and therefore it's not a self selection.
  if (target.kind->kind() != bsgalone::core::EntityKind::SHIP)
  {
    return false;
  }

  const auto sourceShip = m_repositories.playerShipRepository->findOneById(data.sourceDbId);
  const auto targetShip = m_repositories.playerShipRepository->findOneById(target.dbComp().dbId());

  return sourceShip.player == targetShip.player;
}

} // namespace bsgo
