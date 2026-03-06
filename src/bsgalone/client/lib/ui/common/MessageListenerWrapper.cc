
#include "MessageListenerWrapper.hh"

namespace bsgalone::client {

MessageListenerWrapper::MessageListenerWrapper(core::IMessageListener *listener)
  : m_listener(listener)
{
  if (nullptr == m_listener)
  {
    throw std::invalid_argument("Expected non null listener");
  }
}

bool MessageListenerWrapper::isEventRelevant(const core::MessageType &type) const
{
  return m_listener->isEventRelevant(type);
}

void MessageListenerWrapper::onEventReceived(const core::IMessage &message)
{
  m_listener->onEventReceived(message);
}

} // namespace bsgalone::client
