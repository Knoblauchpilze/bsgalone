
#include "ScannedMessage.hh"

namespace bsgo {

ScannedMessage::ScannedMessage(const Uuid &asteroidEntityId)
  : SystemMessage(SystemType::COMPUTER)
  , m_asteroidEntityId(asteroidEntityId)
{}

auto ScannedMessage::asteroidEntityId() const -> Uuid
{
  return m_asteroidEntityId;
}

} // namespace bsgo
