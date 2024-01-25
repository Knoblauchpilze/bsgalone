
#include "MessageListenerWrapper.hh"

namespace pge {

MessageListenerWrapper::MessageListenerWrapper(IMessageListener *listener)
  : m_listener(listener)
{
  if (nullptr == m_listener)
  {
    throw std::invalid_argument("Expected non null listener");
  }
}

bool MessageListenerWrapper::isMessageRelevant(const bsgo::MessageType &type) const
{
  return m_listener->isMessageRelevant(type);
}

void MessageListenerWrapper::onMessageReceived(const bsgo::IMessage &message)
{
  m_listener->onMessageReceived(message);
}

} // namespace pge
