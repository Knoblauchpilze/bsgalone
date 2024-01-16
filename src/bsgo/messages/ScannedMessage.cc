
#include "ScannedMessage.hh"
#include <core_utils/SerializationUtils.hh>

namespace bsgo {

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

auto ScannedMessage::deserialize(std::istream &in) -> std::istream &
{
  utils::deserialize(in, m_messageType);

  utils::deserialize(in, m_asteroidEntityId);

  return in;
}

} // namespace bsgo
