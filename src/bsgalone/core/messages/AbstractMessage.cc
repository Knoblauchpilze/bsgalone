
#include "AbstractMessage.hh"

namespace bsgalone::core {

AbstractMessage::AbstractMessage(const MessageType &type)
  : IMessage(str(type))
  , m_messageType(type)
{}

auto AbstractMessage::type() const -> MessageType
{
  return m_messageType;
}

} // namespace bsgalone::core
