
#include "ConnectionMessage.hh"
#include "SerializationUtils.hh"

namespace bsgo {

ConnectionMessage::ConnectionMessage()
  : ValidatableMessage(MessageType::CONNECTION)
{}

ConnectionMessage::ConnectionMessage(const Uuid clientId)
  : ValidatableMessage(MessageType::CONNECTION)
{
  setClientId(clientId);
}

auto ConnectionMessage::serialize(std::ostream &out) const -> std::ostream &
{
  core::serialize(out, m_messageType);
  core::serialize(out, m_clientId);
  core::serialize(out, m_validated);

  return out;
}

bool ConnectionMessage::deserialize(std::istream &in)
{
  bool ok{true};
  ok &= core::deserialize(in, m_messageType);
  ok &= core::deserialize(in, m_clientId);
  ok &= core::deserialize(in, m_validated);

  return ok;
}

auto ConnectionMessage::clone() const -> IMessagePtr
{
  auto clone = std::make_unique<ConnectionMessage>();
  clone->copyClientIdIfDefined(*this);
  clone->validate(validated());
  return clone;
}

} // namespace bsgo
