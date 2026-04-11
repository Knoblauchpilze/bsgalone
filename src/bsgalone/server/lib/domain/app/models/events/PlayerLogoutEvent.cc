
#include "PlayerLogoutEvent.hh"

namespace bsgalone::server {

PlayerLogoutEvent::PlayerLogoutEvent(const core::Uuid playerDbId)
  : IGameEvent(GameEventType::PLAYER_LOGOUT)
  , m_playerDbId(playerDbId)
{}

auto PlayerLogoutEvent::getPlayerDbId() const -> core::Uuid
{
  return m_playerDbId;
}

auto PlayerLogoutEvent::clone() const -> IGameEventPtr
{
  return std::make_unique<PlayerLogoutEvent>(m_playerDbId);
}

} // namespace bsgalone::server
