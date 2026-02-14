
#include "AbstractMessageQueue.hh"

namespace bsgo {

AbstractMessageQueue::AbstractMessageQueue()
  : IMessageQueue()
{}

void AbstractMessageQueue::addListener(IMessageListenerPtr listener)
{
  if (nullptr == listener)
  {
    throw std::invalid_argument("Expected a non null listener");
  }

  for (const auto &messageType : allMessageTypesAsSet())
  {
    if (listener->isMessageRelevant(messageType))
    {
      m_listenersTable.emplace(messageType, listener.get());
    }
  }

  m_listeners.emplace_back(std::move(listener));
}

} // namespace bsgo
