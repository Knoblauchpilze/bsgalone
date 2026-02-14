
#include "PlayerLoginDataMessage.hh"
#include "SerializationUtils.hh"

namespace bsgo {

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

auto PlayerLoginDataMessage::serialize(std::ostream &out) const -> std::ostream &
{
  core::serialize(out, m_messageType);
  core::serialize(out, m_clientId);

  core::serialize(out, m_faction);
  core::serialize(out, m_activeShipDbId);
  core::serialize(out, m_docked);
  core::serialize(out, m_systemDbId);

  return out;
}

bool PlayerLoginDataMessage::deserialize(std::istream &in)
{
  bool ok{true};
  ok &= core::deserialize(in, m_messageType);
  ok &= core::deserialize(in, m_clientId);

  ok &= core::deserialize(in, m_faction);
  ok &= core::deserialize(in, m_activeShipDbId);
  ok &= core::deserialize(in, m_docked);
  ok &= core::deserialize(in, m_systemDbId);

  return ok;
}

auto PlayerLoginDataMessage::clone() const -> bsgalone::core::IMessagePtr
{
  auto clone = std::make_unique<PlayerLoginDataMessage>();
  clone->setFaction(m_faction);
  clone->setActiveShipDbId(m_activeShipDbId);
  clone->setDocked(m_docked);
  clone->setSystemDbId(m_systemDbId);
  clone->copyClientIdIfDefined(*this);

  return clone;
}

} // namespace bsgo
