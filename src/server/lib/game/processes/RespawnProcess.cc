
#include "RespawnProcess.hh"

namespace bsgo {

RespawnProcess::RespawnProcess(const Repositories &repositories,
                               IMessageQueue *const systemMessageQueue)
  : AbstractProcess(ProcessType::RESPAWN, repositories)
  , m_systemMessageQueue(systemMessageQueue)
{}

void RespawnProcess::update(Coordinator &coordinator, const chrono::TickData &data)
{
  respawnAsteroids(coordinator, data);
}

void RespawnProcess::respawnAsteroids(Coordinator &coordinator, const chrono::TickData &data) const
{
  const auto toRespawn = m_repositories.asteroidRepository->findAllByRespawnTimeUntil(data.tick);
  for (const auto &asteroid : toRespawn)
  {
    respawnAsteroid(coordinator, asteroid);
  }
}

void RespawnProcess::respawnAsteroid(Coordinator & /*coordinator*/, Asteroid asteroid) const
{
  asteroid.health = asteroid.maxHealth;

  info("Respawning asteroid with " + std::to_string(asteroid.health) + " health");
  m_repositories.asteroidRepository->save(asteroid);
  m_repositories.asteroidRepository->deleteRespawn(asteroid.id);
}

} // namespace bsgo
