
#include "AbstractPlayerMessage.hh"

namespace bsgalone::core {

AbstractPlayerMessage::AbstractPlayerMessage(const bsgalone::core::MessageType &type,
                                             const bsgo::Uuid playerDbId)
  : AbstractMessage(type)
  , m_playerDbId(playerDbId)
{}

auto AbstractPlayerMessage::getPlayerDbId() const -> bsgo::Uuid
{
  return m_playerDbId;
}

} // namespace bsgalone::core
