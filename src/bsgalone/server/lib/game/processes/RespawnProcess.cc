
#include "RespawnProcess.hh"
#include "EntityAddedMessage.hh"

namespace bsgalone::server {

RespawnProcess::RespawnProcess(const core::Uuid systemDbId,
                               const core::Repositories &repositories,
                               core::IMessageQueue *const systemMessageQueue)
  : AbstractProcess(ProcessType::RESPAWN, repositories)
  , m_systemDbId(systemDbId)
  , m_systemMessageQueue(systemMessageQueue)
{}

void RespawnProcess::update(core::Coordinator &coordinator, const chrono::TickData &data)
{
  respawnAsteroids(coordinator, data);
  respawnPlayerShips(coordinator, data);
}

void RespawnProcess::respawnAsteroids(core::Coordinator &coordinator,
                                      const chrono::TickData &data) const
{
  const auto toRespawn = m_repositories.asteroidRepository
                           ->findAllBySystemAndRespawnTime(m_systemDbId, data.tick);
  for (const auto &asteroid : toRespawn)
  {
    respawnAsteroid(coordinator, asteroid);
  }
}

void RespawnProcess::respawnPlayerShips(core::Coordinator &coordinator,
                                        const chrono::TickData &data) const
{
  const auto toRespawn = m_repositories.playerShipRepository
                           ->findAllBySystemAndRespawnTime(m_systemDbId, data.tick);
  for (const auto &playerShip : toRespawn)
  {
    respawnPlayerShip(coordinator, playerShip);
  }
}

void RespawnProcess::respawnAsteroid(core::Coordinator & /*coordinator*/,
                                     core::Asteroid asteroid) const
{
  asteroid.health = asteroid.maxHealth;

  m_repositories.asteroidRepository->save(asteroid);
  m_repositories.asteroidRepository->deleteRespawn(asteroid.id);

  auto added = std::make_unique<core::EntityAddedMessage>(asteroid.system);
  core::AsteroidData data{.dbId = asteroid.id};
  added->setAsteroidData(data);
  m_systemMessageQueue->pushEvent(std::move(added));
}

void RespawnProcess::respawnPlayerShip(core::Coordinator & /*coordinator*/,
                                       core::PlayerShip playerShip) const
{
  if (!playerShip.system)
  {
    error("Cannot respawn player ship " + core::str(playerShip.dbId) + " with no system defined");
  }

  playerShip.hullPoints = playerShip.maxHullPoints;
  playerShip.position   = Eigen::Vector3f::Zero();

  m_repositories.playerShipRepository->save(playerShip);
  m_repositories.playerShipRepository->deleteRespawn(playerShip.dbId);

  auto added = std::make_unique<core::EntityAddedMessage>(*playerShip.system);
  core::PlayerShipData data{.dbId = playerShip.dbId};
  added->setShipData(data);
  m_systemMessageQueue->pushEvent(std::move(added));
}

} // namespace bsgalone::server
