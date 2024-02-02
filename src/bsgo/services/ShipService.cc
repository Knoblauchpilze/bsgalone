
#include "ShipService.hh"

namespace bsgo {

ShipService::ShipService(const Repositories &repositories,
                         CoordinatorShPtr coordinator,
                         const DatabaseEntityMapper &entityMapper)
  : AbstractService("ship", repositories)
  , m_coordinator(std::move(coordinator))
  , m_entityMapper(entityMapper)
{}

bool ShipService::trySelectShip(const Uuid shipDbId) const
{
  const auto newActiveShip = m_repositories.playerShipRepository->findOneById(shipDbId);
  if (!newActiveShip.player)
  {
    return false;
  }

  const auto currentActiveShip = m_repositories.playerShipRepository->findOneByPlayerAndActive(
    *newActiveShip.player);

  if (!currentActiveShip.player)
  {
    return false;
  }
  if (newActiveShip.active)
  {
    return false;
  }
  if (*currentActiveShip.player != *newActiveShip.player)
  {
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
    return false;
  }

  auto shipEntity  = m_coordinator->getEntity(*maybeEntityId);
  auto &statusComp = shipEntity.statusComp();

  if (Status::DOCKED == statusComp.status())
  {
    return false;
  }

  const auto ship = m_repositories.playerShipRepository->findOneById(shipDbId);
  m_repositories.systemRepository->updateSystemForShip(shipDbId, *ship.system, true);

  statusComp.setStatus(Status::DOCKED);

  return true;
}

bool ShipService::tryUndock(const Uuid shipDbId) const
{
  const auto maybeEntityId = m_entityMapper.tryGetShipEntityId(shipDbId);
  if (!maybeEntityId)
  {
    return false;
  }

  auto shipEntity  = m_coordinator->getEntity(*maybeEntityId);
  auto &statusComp = shipEntity.statusComp();

  if (statusComp.status() != Status::DOCKED)
  {
    return false;
  }

  const auto ship = m_repositories.playerShipRepository->findOneById(shipDbId);
  m_repositories.systemRepository->updateSystemForShip(shipDbId, *ship.system, false);

  statusComp.setStatus(Status::APPEARING);

  return true;
}

bool ShipService::accelerateShip(const Uuid shipEntityId, const Eigen::Vector3f &acceleration) const
{
  auto shipEntity = m_coordinator->getEntity(shipEntityId);

  if (!shipEntity.exists<OwnerComponent>())
  {
    return false;
  }

  auto &velocity = shipEntity.velocityComp();
  velocity.accelerate(acceleration);

  return true;
}

auto ShipService::tryAcquireTarget(const Uuid shipDbId, const Eigen::Vector3f &position) const
  -> AcquiringResult
{
  const auto maybeEntityId = m_entityMapper.tryGetShipEntityId(shipDbId);
  if (!maybeEntityId)
  {
    return {};
  }

  auto ship          = m_coordinator->getEntity(*maybeEntityId);
  auto maybeTargetId = m_coordinator->getEntityAt(position);

  if (maybeTargetId)
  {
    const auto target = m_coordinator->getEntity(*maybeTargetId);

    if (target.exists<bsgo::StatusComponent>()
        && !bsgo::statusVisibleFromDradis(target.statusComp().status()))
    {
      maybeTargetId.reset();
    }
  }

  updateEntityTarget(ship, maybeTargetId);

  return AcquiringResult{.success = true, .targetDbId = maybeTargetId};
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
