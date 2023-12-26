
#include "SystemMessage.hh"

namespace bsgo {

SystemMessage::SystemMessage(const SystemType &systemType)
  : AbstractMessage(MessageType::SYSTEM)
  , m_systemType(systemType)
{}

auto SystemMessage::systemType() const -> SystemType
{
  return m_systemType;
}

} // namespace bsgo
