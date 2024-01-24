
#include "ConnectionMessage.hh"
#include <core_utils/SerializationUtils.hh>

namespace bsgo {

ConnectionMessage::ConnectionMessage()
  : ValidatableMessage(MessageType::CONNECTION)
{}

ConnectionMessage::ConnectionMessage(const Uuid &clientId)
  : ValidatableMessage(MessageType::CONNECTION)
{
  setClientId(clientId);
}

auto ConnectionMessage::serialize(std::ostream &out) const -> std::ostream &
{
  utils::serialize(out, m_messageType);
  utils::serialize(out, m_clientId);
  utils::serialize(out, m_validated);

  return out;
}

bool ConnectionMessage::deserialize(std::istream &in)
{
  bool ok{true};
  ok &= utils::deserialize(in, m_messageType);
  ok &= utils::deserialize(in, m_clientId);
  ok &= utils::deserialize(in, m_validated);

  return ok;
}

} // namespace bsgo
