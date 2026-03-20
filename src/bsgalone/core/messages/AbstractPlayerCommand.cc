
#include "AbstractPlayerCommand.hh"

namespace bsgalone::core {

AbstractPlayerCommand::AbstractPlayerCommand(const MessageType &type,
                                             const Uuid playerDbId,
                                             const Uuid systemDbId)
  : IMessage(type)
  , m_playerDbId(playerDbId)
  , m_systemDbId(systemDbId)
{}

auto AbstractPlayerCommand::getPlayerDbId() const -> Uuid
{
  return m_playerDbId;
}

auto AbstractPlayerCommand::getSystemDbId() const -> Uuid
{
  return m_systemDbId;
}

} // namespace bsgalone::core
