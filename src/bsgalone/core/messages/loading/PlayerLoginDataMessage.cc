
#include "PlayerLoginDataMessage.hh"
#include "SerializationUtils.hh"

namespace bsgalone::core {

PlayerLoginDataMessage::PlayerLoginDataMessage()
  : NetworkMessage(MessageType::PLAYER_LOGIN_DATA)
{}

auto PlayerLoginDataMessage::getFaction() const -> Faction
{
  return m_faction;
}

auto PlayerLoginDataMessage::getActiveShipDbId() const -> Uuid
{
  return m_activeShipDbId;
}

bool PlayerLoginDataMessage::isDocked() const
{
  return m_docked;
}

auto PlayerLoginDataMessage::getSystemDbId() const -> Uuid
{
  return m_systemDbId;
}

void PlayerLoginDataMessage::setFaction(const Faction faction)
{
  m_faction = faction;
}

void PlayerLoginDataMessage::setActiveShipDbId(const Uuid activeShipDbId)
{
  m_activeShipDbId = activeShipDbId;
}

void PlayerLoginDataMessage::setDocked(const bool docked)
{
  m_docked = docked;
}

void PlayerLoginDataMessage::setSystemDbId(const Uuid systemDbId)
{
  m_systemDbId = systemDbId;
}

auto PlayerLoginDataMessage::clone() const -> IMessagePtr
{
  auto clone = std::make_unique<PlayerLoginDataMessage>();
  clone->setFaction(m_faction);
  clone->setActiveShipDbId(m_activeShipDbId);
  clone->setDocked(m_docked);
  clone->setSystemDbId(m_systemDbId);
  clone->copyClientIdIfDefined(*this);

  return clone;
}

auto operator<<(std::ostream &out, const PlayerLoginDataMessage &message) -> std::ostream &
{
  ::core::serialize(out, message.m_type);
  ::core::serialize(out, message.m_clientId);
  ::core::serialize(out, message.m_faction);
  ::core::serialize(out, message.m_activeShipDbId);
  ::core::serialize(out, message.m_docked);
  ::core::serialize(out, message.m_systemDbId);

  return out;
}

auto operator>>(std::istream &in, PlayerLoginDataMessage &message) -> std::istream &
{
  ::core::deserialize(in, message.m_type);
  ::core::deserialize(in, message.m_clientId);
  ::core::deserialize(in, message.m_faction);
  ::core::deserialize(in, message.m_activeShipDbId);
  ::core::deserialize(in, message.m_docked);
  ::core::deserialize(in, message.m_systemDbId);

  return in;
}

} // namespace bsgalone::core
