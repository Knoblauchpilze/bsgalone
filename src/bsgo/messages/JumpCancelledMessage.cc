
#include "JumpCancelledMessage.hh"
#include <core_utils/SerializationUtils.hh>

namespace bsgo {

JumpCancelledMessage::JumpCancelledMessage()
  : ValidatableMessage(MessageType::JUMP_CANCELLED)
{}

JumpCancelledMessage::JumpCancelledMessage(const Uuid &shipDbId, const Uuid &shipEntityId)
  : ValidatableMessage(MessageType::JUMP_CANCELLED)
  , m_shipDbId(shipDbId)
  , m_shipEntityId(shipEntityId)
{}

auto JumpCancelledMessage::getShipDbId() const -> Uuid
{
  return m_shipDbId;
}

auto JumpCancelledMessage::getShipEntityId() const -> Uuid
{
  return m_shipEntityId;
}

auto JumpCancelledMessage::serialize(std::ostream &out) const -> std::ostream &
{
  utils::serialize(out, m_messageType);
  utils::serialize(out, m_clientId);
  utils::serialize(out, m_validated);

  utils::serialize(out, m_shipDbId);
  utils::serialize(out, m_shipEntityId);

  return out;
}

bool JumpCancelledMessage::deserialize(std::istream &in)
{
  bool ok{true};
  ok &= utils::deserialize(in, m_messageType);
  ok &= utils::deserialize(in, m_clientId);
  ok &= utils::deserialize(in, m_validated);

  ok &= utils::deserialize(in, m_shipDbId);
  ok &= utils::deserialize(in, m_shipEntityId);

  return ok;
}

} // namespace bsgo
