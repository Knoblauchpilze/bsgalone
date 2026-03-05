
#include "MessageListenerWrapper.hh"

namespace pge {

MessageListenerWrapper::MessageListenerWrapper(bsgalone::core::IMessageListener *listener)
  : m_listener(listener)
{
  if (nullptr == m_listener)
  {
    throw std::invalid_argument("Expected non null listener");
  }
}

bool MessageListenerWrapper::isEventRelevant(const bsgalone::core::MessageType &type) const
{
  return m_listener->isEventRelevant(type);
}

void MessageListenerWrapper::onEventReceived(const bsgalone::core::IMessage &message)
{
  m_listener->onEventReceived(message);
}

} // namespace pge
