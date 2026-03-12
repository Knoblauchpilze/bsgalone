
#include "JoinShipMessage.hh"
#include "SerializationUtils.hh"

namespace bsgalone::core {

JoinShipMessage::JoinShipMessage()
  : NetworkMessage(MessageType::JOIN_SHIP)
{}

JoinShipMessage::JoinShipMessage(const Uuid playerDbId, const Uuid shipDbId)
  : NetworkMessage(MessageType::JOIN_SHIP)
  , m_playerDbId(playerDbId)
  , m_shipDbId(shipDbId)
{}

auto JoinShipMessage::getPlayerDbId() const -> Uuid
{
  return m_playerDbId;
}

auto JoinShipMessage::getShipDbId() const -> Uuid
{
  return m_shipDbId;
}

auto JoinShipMessage::clone() const -> IMessagePtr
{
  auto clone = std::make_unique<JoinShipMessage>(m_playerDbId, m_shipDbId);
  clone->copyClientIdIfDefined(*this);

  return clone;
}

auto operator<<(std::ostream &out, const JoinShipMessage &message) -> std::ostream &
{
  ::core::serialize(out, message.m_type);
  ::core::serialize(out, message.m_clientId);
  ::core::serialize(out, message.m_playerDbId);
  ::core::serialize(out, message.m_shipDbId);

  return out;
}

auto operator>>(std::istream &in, JoinShipMessage &message) -> std::istream &
{
  ::core::deserialize(in, message.m_type);
  ::core::deserialize(in, message.m_clientId);
  ::core::deserialize(in, message.m_playerDbId);
  ::core::deserialize(in, message.m_shipDbId);

  return in;
}

} // namespace bsgalone::core
