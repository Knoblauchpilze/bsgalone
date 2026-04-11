
#include "PlayerSignupEvent.hh"

namespace bsgalone::server {

PlayerSignupEvent::PlayerSignupEvent(const net::ClientId clientId)
  : IGameEvent(GameEventType::PLAYER_SIGNUP)
  , m_clientId(clientId)
{}

bool PlayerSignupEvent::successfulSignup() const
{
  return m_playerDbId.has_value() && m_faction.has_value();
}

auto PlayerSignupEvent::getClientId() const -> net::ClientId
{
  return m_clientId;
}

auto PlayerSignupEvent::tryGetPlayerDbId() const -> std::optional<core::Uuid>
{
  return m_playerDbId;
}

auto PlayerSignupEvent::tryGetFaction() const -> std::optional<core::Faction>
{
  return m_faction;
}

void PlayerSignupEvent::setPlayerDbId(const core::Uuid playerDbId)
{
  m_playerDbId = playerDbId;
}

void PlayerSignupEvent::setFaction(const core::Faction faction)
{
  m_faction = faction;
}

auto PlayerSignupEvent::clone() const -> IGameEventPtr
{
  auto out          = std::make_unique<PlayerSignupEvent>(m_clientId);
  out->m_playerDbId = m_playerDbId;
  out->m_faction    = m_faction;

  return out;
}

} // namespace bsgalone::server
