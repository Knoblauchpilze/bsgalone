
#include "MessageQueue.hh"

namespace bsgo {

MessageQueue::MessageQueue()
  : IMessageQueue()
  , utils::CoreObject("queue")
{
  setService("message");
}

void MessageQueue::pushMessage(IMessagePtr message)
{
  m_messages.emplace_back(std::move(message));
}

void MessageQueue::addListener(const MessageType &messageType, IMessageListener *listener)
{
  if (nullptr == listener)
  {
    error("Failed to add listener", "Null listener");
  }

  m_listeners.emplace(messageType, listener);
}

void MessageQueue::processMessages()
{
  warn("should process messages");
}

} // namespace bsgo
