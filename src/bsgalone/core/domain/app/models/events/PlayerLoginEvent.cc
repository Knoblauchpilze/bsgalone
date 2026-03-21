
#include "PlayerLoginEvent.hh"

namespace bsgalone::core {

PlayerLoginEvent::PlayerLoginEvent()
  : IGameEvent(GameEventType::PLAYER_LOGIN)
{}

bool PlayerLoginEvent::successfulLogin() const
{
  return m_playerDbId.has_value() && m_systemDbId.has_value();
}

auto PlayerLoginEvent::tryGetPlayerDbId() const -> std::optional<Uuid>
{
  return m_playerDbId;
}

auto PlayerLoginEvent::tryGetRole() const -> std::optional<GameRole>
{
  return m_role;
}

auto PlayerLoginEvent::tryGetSystemDbId() const -> std::optional<Uuid>
{
  return m_systemDbId;
}

void PlayerLoginEvent::setPlayerDbId(const Uuid playerDbId)
{
  m_playerDbId = playerDbId;
}

void PlayerLoginEvent::setRole(const GameRole role)
{
  m_role = role;
}

void PlayerLoginEvent::setSystemDbId(const Uuid systemDbId)
{
  m_systemDbId = systemDbId;
}

auto PlayerLoginEvent::clone() const -> IGameEventPtr
{
  auto out          = std::make_unique<PlayerLoginEvent>();
  out->m_playerDbId = m_playerDbId;
  out->m_role       = m_role;
  out->m_systemDbId = m_systemDbId;

  return out;
}

} // namespace bsgalone::core
