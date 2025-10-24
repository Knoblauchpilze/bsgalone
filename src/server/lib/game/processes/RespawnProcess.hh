
#pragma once

#include "AbstractProcess.hh"
#include "AsteroidRepository.hh"
#include "IMessageQueue.hh"

namespace bsgo {

class RespawnProcess : public AbstractProcess
{
  public:
  RespawnProcess(const Uuid systemDbId,
                 const Repositories &repositories,
                 IMessageQueue *const systemMessageQueue);
  ~RespawnProcess() override = default;

  void update(Coordinator &coordinator, const chrono::TickData &data) override;

  private:
  Uuid m_systemDbId{};
  IMessageQueue *const m_systemMessageQueue{};

  void respawnAsteroids(Coordinator &coordinator, const chrono::TickData &data) const;

  void respawnAsteroid(Coordinator &coordinator, Asteroid asteroid) const;
};

} // namespace bsgo
