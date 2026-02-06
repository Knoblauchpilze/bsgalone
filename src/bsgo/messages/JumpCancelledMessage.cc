
#include "JumpCancelledMessage.hh"
#include "SerializationUtils.hh"

namespace bsgo {

JumpCancelledMessage::JumpCancelledMessage()
  : NetworkMessage(MessageType::JUMP_CANCELLED)
{}

JumpCancelledMessage::JumpCancelledMessage(const Uuid shipDbId)
  : NetworkMessage(MessageType::JUMP_CANCELLED)
  , m_shipDbId(shipDbId)
{}

auto JumpCancelledMessage::getShipDbId() const -> Uuid
{
  return m_shipDbId;
}

auto JumpCancelledMessage::serialize(std::ostream &out) const -> std::ostream &
{
  core::serialize(out, m_messageType);
  core::serialize(out, m_clientId);

  core::serialize(out, m_shipDbId);

  return out;
}

bool JumpCancelledMessage::deserialize(std::istream &in)
{
  bool ok{true};
  ok &= core::deserialize(in, m_messageType);
  ok &= core::deserialize(in, m_clientId);

  ok &= core::deserialize(in, m_shipDbId);

  return ok;
}

auto JumpCancelledMessage::clone() const -> IMessagePtr
{
  auto clone = std::make_unique<JumpCancelledMessage>(m_shipDbId);
  clone->copyClientIdIfDefined(*this);

  return clone;
}

} // namespace bsgo
