
#include "JoinShipMessage.hh"
#include "SerializationUtils.hh"

namespace bsgo {

JoinShipMessage::JoinShipMessage()
  : NetworkMessage(bsgalone::core::MessageType::JOIN_SHIP)
{}

JoinShipMessage::JoinShipMessage(const Uuid playerDbId, const Uuid shipDbId)
  : NetworkMessage(bsgalone::core::MessageType::JOIN_SHIP)
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

auto JoinShipMessage::serialize(std::ostream &out) const -> std::ostream &
{
  core::serialize(out, m_messageType);
  core::serialize(out, m_clientId);

  core::serialize(out, m_playerDbId);
  core::serialize(out, m_shipDbId);

  return out;
}

bool JoinShipMessage::deserialize(std::istream &in)
{
  bool ok{true};
  ok &= core::deserialize(in, m_messageType);
  ok &= core::deserialize(in, m_clientId);

  ok &= core::deserialize(in, m_playerDbId);
  ok &= core::deserialize(in, m_shipDbId);

  return ok;
}

auto JoinShipMessage::clone() const -> bsgalone::core::IMessagePtr
{
  auto clone = std::make_unique<JoinShipMessage>(m_playerDbId, m_shipDbId);
  clone->copyClientIdIfDefined(*this);

  return clone;
}

} // namespace bsgo
