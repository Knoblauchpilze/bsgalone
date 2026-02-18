
#include "AbstractSystemMessage.hh"

namespace bsgalone::core {

AbstractSystemMessage::AbstractSystemMessage(const MessageType &type, const bsgo::Uuid systemDbId)
  : AbstractMessage(type)
  , m_systemDbId(systemDbId)
{}

auto AbstractSystemMessage::getSystemDbId() const -> bsgo::Uuid
{
  return m_systemDbId;
}

} // namespace bsgalone::core
