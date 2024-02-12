
#include "EntityService.hh"
#include "ShipDataSource.hh"

namespace bsgo {

EntityService::EntityService(const Repositories &repositories,
                             CoordinatorShPtr coordinator,
                             DatabaseEntityMapper &entityMapper)
  : AbstractService("entity", repositories)
  , m_coordinator(std::move(coordinator))
  , m_entityMapper(entityMapper)
{}

bool EntityService::tryCreateShipEntity(const Uuid shipDbId) const
{
  ShipDataSource source{m_repositories};
  source.registerShip(*m_coordinator, shipDbId, m_entityMapper);

  const auto maybeEntityId = m_entityMapper.tryGetShipEntityId(shipDbId);
  if (!maybeEntityId)
  {
    warn("Failed to create entity for ship " + str(shipDbId),
         "Registration did not create an entity for it");
    return false;
  }

  auto shipEntity  = m_coordinator->getEntity(*maybeEntityId);
  auto &statusComp = shipEntity.statusComp();

  const auto ship = m_repositories.playerShipRepository->findOneById(shipDbId);
  m_repositories.systemRepository->updateSystemForShip(shipDbId, *ship.system, false);

  statusComp.setStatus(Status::APPEARING);
  statusComp.resetAppearingTime();

  return true;
}

void EntityService::tryDeleteShipEntity(const Uuid shipDbId) const
{
  const auto maybeEntityId = m_entityMapper.tryGetShipEntityId(shipDbId);

  // Can be that the entity was not created for example if the user is
  // already docked or didn't go out of the outpost at all.
  if (maybeEntityId)
  {
    auto shipEntity = m_coordinator->getEntity(*maybeEntityId);
    if (shipEntity.valid())
    {
      shipEntity.removalComp().markForRemoval();
    }
  }

  m_entityMapper.tryRemoveEntityForShip(shipDbId);
}

void EntityService::tryDeleteAsteroidEntity(const Uuid asteroidDbId) const
{
  const auto maybeEntityId = m_entityMapper.tryGetAsteroidEntityId(asteroidDbId);
  if (!maybeEntityId)
  {
    error("Failed to process deletion of asteroid " + str(asteroidDbId), "No entity registered");
  }

  auto asteroidEntity = m_coordinator->getEntity(*maybeEntityId);
  if (asteroidEntity.valid())
  {
    asteroidEntity.removalComp().markForRemoval();
  }

  m_entityMapper.removeEntityForAsteroid(asteroidDbId);
}

} // namespace bsgo
