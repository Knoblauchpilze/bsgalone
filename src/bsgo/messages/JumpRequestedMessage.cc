
#include "JumpRequestedMessage.hh"
#include "SerializationUtils.hh"

namespace bsgo {

JumpRequestedMessage::JumpRequestedMessage()
  : NetworkMessage(MessageType::JUMP_REQUESTED)
{}

JumpRequestedMessage::JumpRequestedMessage(const Uuid shipDbId, const Uuid systemDbId)
  : NetworkMessage(MessageType::JUMP_REQUESTED)
  , m_shipDbId(shipDbId)
  , m_systemDbId(systemDbId)
{}

auto JumpRequestedMessage::getShipDbId() const -> Uuid
{
  return m_shipDbId;
}

auto JumpRequestedMessage::getJumpSystem() const -> Uuid
{
  return m_systemDbId;
}

auto JumpRequestedMessage::serialize(std::ostream &out) const -> std::ostream &
{
  core::serialize(out, m_messageType);
  core::serialize(out, m_clientId);

  core::serialize(out, m_shipDbId);
  core::serialize(out, m_systemDbId);

  return out;
}

bool JumpRequestedMessage::deserialize(std::istream &in)
{
  bool ok{true};
  ok &= core::deserialize(in, m_messageType);
  ok &= core::deserialize(in, m_clientId);

  ok &= core::deserialize(in, m_shipDbId);
  ok &= core::deserialize(in, m_systemDbId);

  return ok;
}

auto JumpRequestedMessage::clone() const -> IMessagePtr
{
  auto clone = std::make_unique<JumpRequestedMessage>(m_shipDbId, m_systemDbId);
  clone->copyClientIdIfDefined(*this);

  return clone;
}

} // namespace bsgo
