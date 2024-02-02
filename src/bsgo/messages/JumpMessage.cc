
#include "JumpMessage.hh"
#include <core_utils/SerializationUtils.hh>

namespace bsgo {

JumpMessage::JumpMessage()
  : ValidatableMessage(MessageType::JUMP)
{}

JumpMessage::JumpMessage(const Uuid shipDbId)
  : ValidatableMessage(MessageType::JUMP)
  , m_shipDbId(shipDbId)
{}

auto JumpMessage::getShipDbId() const -> Uuid
{
  return m_shipDbId;
}

auto JumpMessage::serialize(std::ostream &out) const -> std::ostream &
{
  utils::serialize(out, m_messageType);
  utils::serialize(out, m_clientId);
  utils::serialize(out, m_validated);

  utils::serialize(out, m_shipDbId);

  return out;
}

bool JumpMessage::deserialize(std::istream &in)
{
  bool ok{true};
  ok &= utils::deserialize(in, m_messageType);
  ok &= utils::deserialize(in, m_clientId);
  ok &= utils::deserialize(in, m_validated);

  ok &= utils::deserialize(in, m_shipDbId);

  return ok;
}

auto JumpMessage::clone() const -> IMessagePtr
{
  auto clone = std::make_unique<JumpMessage>(m_shipDbId);
  clone->copyClientIdIfDefined(*this);
  clone->validate(validated());

  return clone;
}

} // namespace bsgo
