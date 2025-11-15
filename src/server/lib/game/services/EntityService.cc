
#include "EntityService.hh"
#include "AsteroidDataSource.hh"
#include "PlayerDataSource.hh"
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
  handlePlayerCreationForShip(shipDbId);

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
      performEntityDeletion(shipEntity);
    }
  }

  m_entityMapper.tryRemoveEntityForShip(shipDbId);
  handlePlayerDeletionForShip(shipDbId);
}

bool EntityService::tryCreateAsteroidEntity(const Uuid asteroidDbId) const
{
  AsteroidDataSource source{m_repositories};
  source.registerAsteroid(*m_coordinator, asteroidDbId, m_entityMapper);

  const auto maybeEntityId = m_entityMapper.tryGetAsteroidEntityId(asteroidDbId);
  if (!maybeEntityId)
  {
    warn("Failed to create entity for asteroid " + str(asteroidDbId),
         "Registration did not create an entity for it");
    return false;
  }

  return true;
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
    performEntityDeletion(asteroidEntity);
  }

  m_entityMapper.removeEntityForAsteroid(asteroidDbId);
}

auto EntityService::getPlayerDbIdForShip(const Uuid shipDbId) const -> Uuid
{
  const auto ship = m_repositories.playerShipRepository->findOneById(shipDbId);
  return ship.player;
}

void EntityService::handlePlayerCreationForShip(const Uuid &shipDbId) const
{
  const auto ship = m_repositories.playerShipRepository->findOneById(shipDbId);

  const auto maybePlayerEntityId = m_entityMapper.tryGetPlayerEntityId(ship.player);
  if (maybePlayerEntityId)
  {
    return;
  }

  PlayerDataSource source{m_repositories};
  source.registerPlayer(*m_coordinator, ship.player, m_entityMapper);

  info("Registered player " + str(ship.player) + " in system " + str(*ship.system));
}

void EntityService::handlePlayerDeletionForShip(const Uuid &shipDbId) const
{
  const auto ship = m_repositories.playerShipRepository->findOneById(shipDbId);

  const auto maybePlayerEntityId = m_entityMapper.tryGetPlayerEntityId(ship.player);
  if (!maybePlayerEntityId)
  {
    return;
  }

  m_coordinator->deleteEntity(*maybePlayerEntityId);
  m_entityMapper.removeEntityForPlayer(ship.player);

  info("Removed player " + str(ship.player) + " from system " + str(*ship.system));
}

void EntityService::performEntityDeletion(Entity &entity) const
{
  entity.removalComp().markForRemoval();
}

} // namespace bsgo
