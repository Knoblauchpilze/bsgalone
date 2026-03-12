
#include "ConnectionMessage.hh"
#include "SerializationUtils.hh"

namespace bsgalone::core {

ConnectionMessage::ConnectionMessage()
  : IMessage(MessageType::CONNECTION)
{}

auto ConnectionMessage::clone() const -> IMessagePtr
{
  return std::make_unique<ConnectionMessage>();
}

} // namespace bsgalone::core
