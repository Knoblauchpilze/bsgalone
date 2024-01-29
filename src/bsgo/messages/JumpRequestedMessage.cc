
#include "JumpRequestedMessage.hh"
#include <core_utils/SerializationUtils.hh>

namespace bsgo {

JumpRequestedMessage::JumpRequestedMessage()
  : ValidatableMessage(MessageType::JUMP_REQUESTED)
{}

JumpRequestedMessage::JumpRequestedMessage(const Uuid shipDbId,
                                           const Uuid shipEntityId,
                                           const Uuid systemDbId)
  : ValidatableMessage(MessageType::JUMP_REQUESTED)
  , m_shipDbId(shipDbId)
  , m_shipEntityId(shipEntityId)
  , m_systemDbId(systemDbId)
{}

auto JumpRequestedMessage::getShipDbId() const -> Uuid
{
  return m_shipDbId;
}

auto JumpRequestedMessage::getShipEntityId() const -> Uuid
{
  return m_shipEntityId;
}

auto JumpRequestedMessage::getJumpSystem() const -> Uuid
{
  return m_systemDbId;
}

auto JumpRequestedMessage::serialize(std::ostream &out) const -> std::ostream &
{
  utils::serialize(out, m_messageType);
  utils::serialize(out, m_clientId);
  utils::serialize(out, m_validated);

  utils::serialize(out, m_shipDbId);
  utils::serialize(out, m_shipEntityId);
  utils::serialize(out, m_systemDbId);

  return out;
}

bool JumpRequestedMessage::deserialize(std::istream &in)
{
  bool ok{true};
  ok &= utils::deserialize(in, m_messageType);
  ok &= utils::deserialize(in, m_clientId);
  ok &= utils::deserialize(in, m_validated);

  ok &= utils::deserialize(in, m_shipDbId);
  ok &= utils::deserialize(in, m_shipEntityId);
  ok &= utils::deserialize(in, m_systemDbId);

  return ok;
}

auto JumpRequestedMessage::clone() const -> IMessagePtr
{
  auto clone = std::make_unique<JumpRequestedMessage>(m_shipDbId, m_shipEntityId, m_systemDbId);
  clone->copyClientIdIfDefined(*this);
  clone->validate(validated());

  return clone;
}

} // namespace bsgo
