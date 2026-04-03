
#include "PlayerSignupEvent.hh"

namespace bsgalone::core {

PlayerSignupEvent::PlayerSignupEvent(const net::ClientId clientId)
  : IGameEvent(GameEventType::PLAYER_SIGNUP)
  , m_clientId(clientId)
{}

bool PlayerSignupEvent::successfulSignup() const
{
  return m_playerDbId.has_value() && m_faction.has_value() && m_systemDbId.has_value();
}

auto PlayerSignupEvent::getClientId() const -> net::ClientId
{
  return m_clientId;
}

auto PlayerSignupEvent::tryGetPlayerDbId() const -> std::optional<Uuid>
{
  return m_playerDbId;
}

auto PlayerSignupEvent::tryGetFaction() const -> std::optional<Faction>
{
  return m_faction;
}

auto PlayerSignupEvent::tryGetSystemDbId() const -> std::optional<Uuid>
{
  return m_systemDbId;
}

void PlayerSignupEvent::setPlayerDbId(const Uuid playerDbId)
{
  m_playerDbId = playerDbId;
}

void PlayerSignupEvent::setFaction(const Faction faction)
{
  m_faction = faction;
}

void PlayerSignupEvent::setSystemDbId(const Uuid systemDbId)
{
  m_systemDbId = systemDbId;
}

auto PlayerSignupEvent::clone() const -> IGameEventPtr
{
  auto out          = std::make_unique<PlayerSignupEvent>(m_clientId);
  out->m_playerDbId = m_playerDbId;
  out->m_faction    = m_faction;
  out->m_systemDbId = m_systemDbId;

  return out;
}

} // namespace bsgalone::core
