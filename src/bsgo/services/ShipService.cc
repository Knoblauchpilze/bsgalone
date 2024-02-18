
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
  const auto newActiveShip = m_repositories.playerShipRepository->findOneById(shipDbId);
  if (!newActiveShip.player)
  {
    warn("Failed to select ship " + str(shipDbId), "Ship does not belong to any player");
    return false;
  }

  const auto currentActiveShip = m_repositories.playerShipRepository->findOneByPlayerAndActive(
    *newActiveShip.player);

  if (!currentActiveShip.player)
  {
    warn("Failed to select ship " + str(shipDbId),
         "Current active ship " + str(currentActiveShip.id) + " does not belong to a player");
    return false;
  }
  if (newActiveShip.active)
  {
    warn("Failed to select ship " + str(shipDbId), "Ship is already active");
    return false;
  }
  if (*currentActiveShip.player != *newActiveShip.player)
  {
    warn("Failed to select ship " + str(shipDbId),
         "Current ship belongs to " + str(*currentActiveShip.player) + " but new one belongs to "
           + str(*newActiveShip.player));
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

  if (ship.exists<NetworkComponent>())
  {
    ship.networkComp().markForSync();
  }

  return true;
}

auto ShipService::tryAcquireTarget(const Uuid shipDbId, const Eigen::Vector3f &position) const
  -> AcquiringResult
{
  const auto maybeEntityId = m_entityMapper.tryGetShipEntityId(shipDbId);
  if (!maybeEntityId)
  {
    warn("Failed to acquire target for ship " + str(shipDbId), "No entity attached to it");
    return {};
  }

  auto ship          = m_coordinator->getEntity(*maybeEntityId);
  auto maybeTargetId = m_coordinator->getEntityAt(position);

  std::optional<Uuid> maybeTargetDbId{};
  std::optional<EntityKind> maybeTargetKind{};

  if (maybeTargetId)
  {
    const auto target = m_coordinator->getEntity(*maybeTargetId);

    if (target.exists<StatusComponent>() && !statusVisibleFromDradis(target.statusComp().status()))
    {
      maybeTargetId.reset();
    }
  }

  if (maybeTargetId)
  {
    const auto target = m_coordinator->getEntity(*maybeTargetId);

    debug("Determined target " + target.str());

    maybeTargetKind = target.kind->kind();
    maybeTargetDbId = target.dbComp().dbId();
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

} // namespace bsgo
