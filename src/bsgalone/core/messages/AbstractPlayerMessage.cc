
#include "AbstractPlayerMessage.hh"

namespace bsgalone::core {

AbstractPlayerMessage::AbstractPlayerMessage(const MessageType &type,
                                             const bsgo::Uuid playerDbId,
                                             const bsgo::Uuid systemDbId)
  : AbstractMessage(type)
  , m_playerDbId(playerDbId)
  , m_systemDbId(systemDbId)
{}

auto AbstractPlayerMessage::getPlayerDbId() const -> bsgo::Uuid
{
  return m_playerDbId;
}

auto AbstractPlayerMessage::getSystemDbId() const -> bsgo::Uuid
{
  return m_systemDbId;
}

} // namespace bsgalone::core
