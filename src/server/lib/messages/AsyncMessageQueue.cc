
#include "AsyncMessageQueue.hh"

namespace bsgo {

AsyncMessageQueue::AsyncMessageQueue()
  : IMessageQueue()
  , utils::CoreObject("async")
{
  addModule("queue");
  setService("message");
}

void AsyncMessageQueue::pushMessage(IMessagePtr message)
{
  m_messageQueue.pushMessage(std::move(message));
}

void AsyncMessageQueue::addListener(IMessageListener *listener)
{
  if (nullptr == listener)
  {
    error("Failed to add listener", "Null listener");
  }

  for (const auto &messageType : allMessageTypes())
  {
    if (listener->isMessageRelevant(messageType))
    {
      m_listeners.emplace(messageType, listener);
    }
  }
}

void AsyncMessageQueue::processMessages()
{
  /// TODO: Handle async thread.
  warn("should process all messages");
}

} // namespace bsgo
