
#include "ScannedMessage.hh"
#include <core_utils/SerializationUtils.hh>

namespace bsgo {

ScannedMessage::ScannedMessage()
  : NetworkMessage(MessageType::SCANNED)
{}

ScannedMessage::ScannedMessage(const Uuid asteroidEntityId)
  : NetworkMessage(MessageType::SCANNED)
  , m_asteroidEntityId(asteroidEntityId)
{}

auto ScannedMessage::asteroidEntityId() const -> Uuid
{
  return m_asteroidEntityId;
}

auto ScannedMessage::serialize(std::ostream &out) const -> std::ostream &
{
  utils::serialize(out, m_messageType);
  utils::serialize(out, m_clientId);

  utils::serialize(out, m_asteroidEntityId);

  return out;
}

bool ScannedMessage::deserialize(std::istream &in)
{
  bool ok{true};
  ok &= utils::deserialize(in, m_messageType);
  ok &= utils::deserialize(in, m_clientId);

  ok &= utils::deserialize(in, m_asteroidEntityId);

  return ok;
}

auto ScannedMessage::clone() const -> IMessagePtr
{
  auto clone = std::make_unique<ScannedMessage>(m_asteroidEntityId);
  clone->copyClientIdIfDefined(*this);

  return clone;
}

} // namespace bsgo
