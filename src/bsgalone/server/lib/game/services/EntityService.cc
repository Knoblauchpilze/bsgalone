
#include "EntityService.hh"
#include "AsteroidDataSource.hh"
#include "PlayerDataSource.hh"
#include "ShipDataSource.hh"

namespace bsgalone::server {

EntityService::EntityService(const core::Repositories &repositories,
                             core::CoordinatorShPtr coordinator,
                             core::DatabaseEntityMapper &entityMapper)
  : AbstractService("entity", repositories)
  , m_coordinator(std::move(coordinator))
  , m_entityMapper(entityMapper)
{}

bool EntityService::tryCreateShipEntity(const core::Uuid shipDbId) const
{
  handlePlayerCreationForShip(shipDbId);

  core::ShipDataSource source{m_repositories};
  source.registerShip(*m_coordinator, shipDbId, m_entityMapper);

  const auto maybeEntityId = m_entityMapper.tryGetShipEntityId(shipDbId);
  if (!maybeEntityId)
  {
    warn("Failed to create entity for ship " + core::str(shipDbId),
         "Registration did not create an entity for it");
    return false;
  }

  auto shipEntity  = m_coordinator->getEntity(*maybeEntityId);
  auto &statusComp = shipEntity.statusComp();

  const auto ship = m_repositories.playerShipRepository->findOneById(shipDbId);
  m_repositories.systemRepository->updateSystemForShip(shipDbId, *ship.system, false);

  statusComp.setStatus(core::Status::APPEARING);
  statusComp.resetAppearingTime();

  return true;
}

void EntityService::tryDeleteShipEntity(const core::Uuid shipDbId) const
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

bool EntityService::tryCreateAsteroidEntity(const core::Uuid asteroidDbId) const
{
  core::AsteroidDataSource source{m_repositories};
  source.registerAsteroid(*m_coordinator, asteroidDbId, m_entityMapper);

  const auto maybeEntityId = m_entityMapper.tryGetAsteroidEntityId(asteroidDbId);
  if (!maybeEntityId)
  {
    warn("Failed to create entity for asteroid " + core::str(asteroidDbId),
         "Registration did not create an entity for it");
    return false;
  }

  return true;
}

void EntityService::tryDeleteAsteroidEntity(const core::Uuid asteroidDbId) const
{
  const auto maybeEntityId = m_entityMapper.tryGetAsteroidEntityId(asteroidDbId);
  if (!maybeEntityId)
  {
    error("Failed to process deletion of asteroid " + core::str(asteroidDbId),
          "No entity registered");
  }

  auto asteroidEntity = m_coordinator->getEntity(*maybeEntityId);
  if (asteroidEntity.valid())
  {
    performEntityDeletion(asteroidEntity);
  }

  m_entityMapper.removeEntityForAsteroid(asteroidDbId);
}

auto EntityService::getPlayerDbIdForShip(const core::Uuid shipDbId) const -> core::Uuid
{
  const auto ship = m_repositories.playerShipRepository->findOneById(shipDbId);
  return ship.player;
}

void EntityService::handlePlayerCreationForShip(const core::Uuid &shipDbId) const
{
  const auto ship = m_repositories.playerShipRepository->findOneById(shipDbId);

  const auto maybePlayerEntityId = m_entityMapper.tryGetPlayerEntityId(ship.player);
  if (maybePlayerEntityId)
  {
    return;
  }

  core::PlayerDataSource source{m_repositories};
  source.registerPlayer(*m_coordinator, ship.player, m_entityMapper);

  info("Registered player " + core::str(ship.player) + " in system " + core::str(*ship.system));
}

void EntityService::handlePlayerDeletionForShip(const core::Uuid &shipDbId) const
{
  const auto ship = m_repositories.playerShipRepository->findOneById(shipDbId);

  const auto maybePlayerEntityId = m_entityMapper.tryGetPlayerEntityId(ship.player);
  if (!maybePlayerEntityId)
  {
    return;
  }

  m_coordinator->deleteEntity(*maybePlayerEntityId);
  m_entityMapper.removeEntityForPlayer(ship.player);

  info("Removed player " + core::str(ship.player) + " from system " + core::str(*ship.system));
}

void EntityService::performEntityDeletion(core::Entity &entity) const
{
  entity.removalComp().markForRemoval();
}

} // namespace bsgalone::server
