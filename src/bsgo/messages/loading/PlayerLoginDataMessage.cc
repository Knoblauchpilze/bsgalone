
#include "PlayerLoginDataMessage.hh"
#include "SerializationUtils.hh"

namespace bsgo {

PlayerLoginDataMessage::PlayerLoginDataMessage()
  : NetworkMessage(MessageType::PLAYER_LOGIN_DATA)
{}

PlayerLoginDataMessage::PlayerLoginDataMessage(const Faction faction,
                                               const Uuid activeShipDbId,
                                               const bool docked)
  : NetworkMessage(MessageType::PLAYER_LOGIN_DATA)
  , m_faction(faction)
  , m_activeShipDbId(activeShipDbId)
  , m_docked(docked)
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

auto PlayerLoginDataMessage::serialize(std::ostream &out) const -> std::ostream &
{
  core::serialize(out, m_messageType);
  core::serialize(out, m_clientId);

  core::serialize(out, m_faction);
  core::serialize(out, m_activeShipDbId);
  core::serialize(out, m_docked);

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

  return ok;
}

auto PlayerLoginDataMessage::clone() const -> IMessagePtr
{
  auto clone = std::make_unique<PlayerLoginDataMessage>(m_faction, m_activeShipDbId, m_docked);
  clone->copyClientIdIfDefined(*this);

  return clone;
}

} // namespace bsgo
