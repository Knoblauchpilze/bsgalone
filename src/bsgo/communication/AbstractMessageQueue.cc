
#include "AbstractMessageQueue.hh"

namespace bsgo {

AbstractMessageQueue::AbstractMessageQueue()
  : IMessageQueue()
{}

void AbstractMessageQueue::addListener(IMessageListener *listener)
{
  if (nullptr == listener)
  {
    throw std::invalid_argument("Expected a non null listener");
  }

  for (const auto &messageType : allMessageTypes())
  {
    if (listener->isMessageRelevant(messageType))
    {
      m_listeners.emplace(messageType, listener);
    }
  }
}

} // namespace bsgo
