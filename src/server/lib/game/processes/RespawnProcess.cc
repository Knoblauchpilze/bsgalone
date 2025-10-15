
#include "RespawnProcess.hh"

namespace bsgo {

RespawnProcess::RespawnProcess(const Repositories &repositories)
  : AbstractProcess(ProcessType::RESPAWN, repositories)
{}

void RespawnProcess::update(Coordinator &coordinator, const chrono::TickData & /*data*/) const
{
  respawnAsteroids(coordinator);
}

void RespawnProcess::respawnAsteroids(Coordinator & /*coordinator*/) const
{
  const auto toRespawn = m_repositories.asteroidRepository->findAllByRespawnTimeUntil(core::now());
  for (const auto &asteroid : toRespawn)
  {
    // TODO: Handle respawn process
    warn("Should respawn asteroid " + str(asteroid.id));
  }
}

} // namespace bsgo
