
#include "ConnectionMessage.hh"
#include <core_utils/SerializationUtils.hh>

namespace bsgo {

ConnectionMessage::ConnectionMessage()
  : ValidatableMessage(MessageType::CONNECTION)
{}

ConnectionMessage::ConnectionMessage(const Uuid &clientId)
  : ValidatableMessage(MessageType::CONNECTION)
  , m_clientId(clientId)
{}

auto ConnectionMessage::getClientId() const -> Uuid
{
  return m_clientId;
}

auto ConnectionMessage::serialize(std::ostream &out) const -> std::ostream &
{
  utils::serialize(out, m_messageType);
  utils::serialize(out, m_validated);

  utils::serialize(out, m_clientId);

  return out;
}

bool ConnectionMessage::deserialize(std::istream &in)
{
  bool ok{true};
  ok &= utils::deserialize(in, m_messageType);
  ok &= utils::deserialize(in, m_validated);

  ok &= utils::deserialize(in, m_clientId);

  return ok;
}

} // namespace bsgo
