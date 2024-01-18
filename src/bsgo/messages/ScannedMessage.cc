
#include "ScannedMessage.hh"
#include <core_utils/SerializationUtils.hh>

namespace bsgo {

ScannedMessage::ScannedMessage()
  : AbstractMessage(MessageType::SCANNED)
{}

ScannedMessage::ScannedMessage(const Uuid &asteroidEntityId)
  : AbstractMessage(MessageType::SCANNED)
  , m_asteroidEntityId(asteroidEntityId)
{}

auto ScannedMessage::asteroidEntityId() const -> Uuid
{
  return m_asteroidEntityId;
}

auto ScannedMessage::serialize(std::ostream &out) const -> std::ostream &
{
  utils::serialize(out, m_messageType);

  utils::serialize(out, m_asteroidEntityId);

  return out;
}

bool ScannedMessage::deserialize(std::istream &in)
{
  bool ok{true};
  ok &= utils::deserialize(in, m_messageType);

  ok &= utils::deserialize(in, m_asteroidEntityId);

  return ok;
}

} // namespace bsgo
