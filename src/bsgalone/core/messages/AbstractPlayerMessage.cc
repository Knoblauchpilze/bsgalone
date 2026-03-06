
#include "AbstractPlayerMessage.hh"

namespace bsgalone::core {

AbstractPlayerMessage::AbstractPlayerMessage(const MessageType &type,
                                             const Uuid playerDbId,
                                             const Uuid systemDbId)
  : AbstractMessage(type)
  , m_playerDbId(playerDbId)
  , m_systemDbId(systemDbId)
{}

auto AbstractPlayerMessage::getPlayerDbId() const -> Uuid
{
  return m_playerDbId;
}

auto AbstractPlayerMessage::getSystemDbId() const -> Uuid
{
  return m_systemDbId;
}

} // namespace bsgalone::core
