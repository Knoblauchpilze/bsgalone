
#include "AbstractSystemMessage.hh"

namespace bsgo {

AbstractSystemMessage::AbstractSystemMessage(const MessageType &type, const Uuid systemDbId)
  : AbstractMessage(type)
  , m_systemDbId(systemDbId)
{}

auto AbstractSystemMessage::getSystemDbId() const -> Uuid
{
  return m_systemDbId;
}

} // namespace bsgo
