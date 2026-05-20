
#pragma once

#include "Asteroid.hh"
#include "IGameEvent.hh"
#include "System.hh"
#include "Uuid.hh"
#include <vector>

namespace bsgalone::server {

class PlayerUndockEvent : public IGameEvent
{
  public:
  explicit PlayerUndockEvent(const core::Uuid playerDbId,
                             System system,
                             std::vector<core::Asteroid> asteroids);
  ~PlayerUndockEvent() override = default;

  auto getPlayerDbId() const -> core::Uuid;
  auto getSystem() const -> System;
  auto getAsteroids() const -> const std::vector<core::Asteroid> &;

  auto clone() const -> IGameEventPtr override;

  private:
  core::Uuid m_playerDbId{};
  System m_system{};
  std::vector<core::Asteroid> m_asteroids{};
};

} // namespace bsgalone::server
