
#pragma once

#include "AbstractProcess.hh"
#include "AsteroidRepository.hh"
#include "IMessageQueue.hh"

namespace bsgalone::server {

class RespawnProcess : public AbstractProcess
{
  public:
  RespawnProcess(const core::Uuid systemDbId,
                 const core::Repositories &repositories,
                 core::IMessageQueue *const systemMessageQueue);
  ~RespawnProcess() override = default;

  void update(core::Coordinator &coordinator, const chrono::TickData &data) override;

  private:
  core::Uuid m_systemDbId{};
  core::IMessageQueue *const m_systemMessageQueue{};

  void respawnAsteroids(core::Coordinator &coordinator, const chrono::TickData &data) const;
  void respawnPlayerShips(core::Coordinator &coordinator, const chrono::TickData &data) const;

  void respawnAsteroid(core::Coordinator &coordinator, core::Asteroid asteroid) const;
  void respawnPlayerShip(core::Coordinator &coordinator, core::PlayerShip playerShip) const;
};

} // namespace bsgalone::server
