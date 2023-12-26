
#include "ScannedMessage.hh"

namespace bsgo {

ScannedMessage::ScannedMessage(const Uuid &asteroidDbId)
  : SystemMessage(SystemType::COMPUTER)
  , m_asteroidDbId(asteroidDbId)
{}

auto ScannedMessage::asteroidDbId() const -> Uuid
{
  return m_asteroidDbId;
}

} // namespace bsgo
