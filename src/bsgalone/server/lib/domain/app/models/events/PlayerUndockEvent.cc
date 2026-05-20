
#include "PlayerUndockEvent.hh"

namespace bsgalone::server {

PlayerUndockEvent::PlayerUndockEvent(const core::Uuid playerDbId,
                                     System system,
                                     std::vector<core::Asteroid> asteroids)
  : IGameEvent(GameEventType::PLAYER_UNDOCK)
  , m_playerDbId(playerDbId)
  , m_system(std::move(system))
  , m_asteroids(std::move(asteroids))
{}

auto PlayerUndockEvent::getPlayerDbId() const -> core::Uuid
{
  return m_playerDbId;
}

auto PlayerUndockEvent::getSystem() const -> System
{
  return m_system;
}

auto PlayerUndockEvent::getAsteroids() const -> const std::vector<core::Asteroid> &
{
  return m_asteroids;
}

auto PlayerUndockEvent::clone() const -> IGameEventPtr
{
  return std::make_unique<PlayerUndockEvent>(m_playerDbId, m_system, m_asteroids);
}

} // namespace bsgalone::server
