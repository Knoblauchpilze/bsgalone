
#include "AbstractPlayerMessage.hh"

namespace bsgo {

AbstractPlayerMessage::AbstractPlayerMessage(const MessageType &type, const Uuid playerDbId)
  : AbstractMessage(type)
  , m_playerDbId(playerDbId)
{}

auto AbstractPlayerMessage::getPlayerDbId() const -> Uuid
{
  return m_playerDbId;
}

} // namespace bsgo
