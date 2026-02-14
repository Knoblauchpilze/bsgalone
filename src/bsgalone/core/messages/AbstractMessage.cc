
#include "AbstractMessage.hh"

namespace bsgalone::core {

AbstractMessage::AbstractMessage(const bsgo::MessageType &type)
  : IMessage(str(type))
  , m_messageType(type)
{}

auto AbstractMessage::type() const -> bsgo::MessageType
{
  return m_messageType;
}

} // namespace bsgalone::core
