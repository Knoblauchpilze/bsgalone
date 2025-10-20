
#include "RespawnProcess.hh"

namespace bsgo {

RespawnProcess::RespawnProcess(const Repositories &repositories)
  : AbstractProcess(ProcessType::RESPAWN, repositories)
{}

void RespawnProcess::update(Coordinator &coordinator, const chrono::TickData &data)
{
  respawnAsteroids(coordinator, data);
}

void RespawnProcess::respawnAsteroids(Coordinator & /*coordinator*/,
                                      const chrono::TickData &data) const
{
  const auto toRespawn = m_repositories.asteroidRepository->findAllByRespawnTimeUntil(data.tick);
  for (const auto &asteroid : toRespawn)
  {
    // TODO: Handle respawn process
    warn("Should respawn asteroid " + str(asteroid.id));
  }
}

} // namespace bsgo
