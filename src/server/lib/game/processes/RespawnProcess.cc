
#include "RespawnProcess.hh"
#include "EntityAddedMessage.hh"

namespace bsgo {

RespawnProcess::RespawnProcess(const Uuid systemDbId,
                               const Repositories &repositories,
                               bsgalone::core::IMessageQueue *const systemMessageQueue)
  : AbstractProcess(ProcessType::RESPAWN, repositories)
  , m_systemDbId(systemDbId)
  , m_systemMessageQueue(systemMessageQueue)
{}

void RespawnProcess::update(Coordinator &coordinator, const chrono::TickData &data)
{
  respawnAsteroids(coordinator, data);
  respawnPlayerShips(coordinator, data);
}

void RespawnProcess::respawnAsteroids(Coordinator &coordinator, const chrono::TickData &data) const
{
  const auto toRespawn = m_repositories.asteroidRepository
                           ->findAllBySystemAndRespawnTime(m_systemDbId, data.tick);
  for (const auto &asteroid : toRespawn)
  {
    respawnAsteroid(coordinator, asteroid);
  }
}

void RespawnProcess::respawnPlayerShips(Coordinator &coordinator, const chrono::TickData &data) const
{
  const auto toRespawn = m_repositories.playerShipRepository
                           ->findAllBySystemAndRespawnTime(m_systemDbId, data.tick);
  for (const auto &playerShip : toRespawn)
  {
    respawnPlayerShip(coordinator, playerShip);
  }
}

void RespawnProcess::respawnAsteroid(Coordinator & /*coordinator*/, Asteroid asteroid) const
{
  asteroid.health = asteroid.maxHealth;

  m_repositories.asteroidRepository->save(asteroid);
  m_repositories.asteroidRepository->deleteRespawn(asteroid.id);

  auto added = std::make_unique<EntityAddedMessage>(asteroid.system);
  AsteroidData data{.dbId = asteroid.id};
  added->setAsteroidData(data);
  m_systemMessageQueue->pushMessage(std::move(added));
}

void RespawnProcess::respawnPlayerShip(Coordinator & /*coordinator*/, PlayerShip playerShip) const
{
  if (!playerShip.system)
  {
    error("Cannot respawn player ship " + str(playerShip.id) + " with no system defined");
  }

  playerShip.hullPoints = playerShip.maxHullPoints;
  playerShip.position   = Eigen::Vector3f::Zero();

  m_repositories.playerShipRepository->save(playerShip);
  m_repositories.playerShipRepository->deleteRespawn(playerShip.id);

  auto added = std::make_unique<EntityAddedMessage>(*playerShip.system);
  PlayerShipData data{.dbId = playerShip.id};
  added->setShipData(data);
  m_systemMessageQueue->pushMessage(std::move(added));
}

} // namespace bsgo
