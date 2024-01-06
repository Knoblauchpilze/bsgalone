
#include "ScannedMessage.hh"

namespace bsgo {

ScannedMessage::ScannedMessage(const Uuid &asteroidEntityId)
  : AbstractMessage(MessageType::SCANNED)
  , m_asteroidEntityId(asteroidEntityId)
{}

auto ScannedMessage::asteroidEntityId() const -> Uuid
{
  return m_asteroidEntityId;
}

} // namespace bsgo
