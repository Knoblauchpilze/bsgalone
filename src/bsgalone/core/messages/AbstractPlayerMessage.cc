
#include "AbstractPlayerMessage.hh"

namespace bsgalone::core {

AbstractPlayerMessage::AbstractPlayerMessage(const MessageType &type, const Uuid playerDbId)
  : IMessage(type)
  , m_playerDbId(playerDbId)
{}

auto AbstractPlayerMessage::getPlayerDbId() const -> Uuid
{
  return m_playerDbId;
}

} // namespace bsgalone::core
