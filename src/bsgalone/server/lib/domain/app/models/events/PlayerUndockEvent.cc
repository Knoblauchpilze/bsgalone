
#include "PlayerUndockEvent.hh"

namespace bsgalone::server {

PlayerUndockEvent::PlayerUndockEvent(const core::Uuid playerDbId)
  : IGameEvent(GameEventType::PLAYER_UNDOCK)
  , m_playerDbId(playerDbId)
{}

auto PlayerUndockEvent::getPlayerDbId() const -> core::Uuid
{
  return m_playerDbId;
}

auto PlayerUndockEvent::clone() const -> IGameEventPtr
{
  return std::make_unique<PlayerUndockEvent>(m_playerDbId);
}

} // namespace bsgalone::server
