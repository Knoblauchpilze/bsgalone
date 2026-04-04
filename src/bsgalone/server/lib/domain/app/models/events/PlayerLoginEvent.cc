
#include "PlayerLoginEvent.hh"

namespace bsgalone::core {

PlayerLoginEvent::PlayerLoginEvent(const net::ClientId clientId)
  : IGameEvent(GameEventType::PLAYER_LOGIN)
  , m_clientId(clientId)
{}

bool PlayerLoginEvent::successfulLogin() const
{
  return m_playerDbId.has_value() && m_role.has_value();
}

auto PlayerLoginEvent::getClientId() const -> net::ClientId
{
  return m_clientId;
}

auto PlayerLoginEvent::tryGetPlayerDbId() const -> std::optional<Uuid>
{
  return m_playerDbId;
}

auto PlayerLoginEvent::tryGetRole() const -> std::optional<GameRole>
{
  return m_role;
}

void PlayerLoginEvent::setPlayerDbId(const Uuid playerDbId)
{
  m_playerDbId = playerDbId;
}

void PlayerLoginEvent::setRole(const GameRole role)
{
  m_role = role;
}

auto PlayerLoginEvent::clone() const -> IGameEventPtr
{
  auto out          = std::make_unique<PlayerLoginEvent>(m_clientId);
  out->m_playerDbId = m_playerDbId;
  out->m_role       = m_role;

  return out;
}

} // namespace bsgalone::core
