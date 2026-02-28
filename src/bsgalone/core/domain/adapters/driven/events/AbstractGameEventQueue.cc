
#include "AbstractGameEventQueue.hh"

namespace bsgalone::core {

void AbstractGameEventQueue::addListener(IGameEventListenerPtr listener)
{
  if (nullptr == listener)
  {
    throw std::invalid_argument("Expected a non null listener");
  }

  for (const auto &eventType : allGameEventTypesAsSet())
  {
    if (listener->isEventRelevant(eventType))
    {
      m_listenersTable.emplace(eventType, listener.get());
    }
  }

  m_listeners.emplace_back(std::move(listener));
}

} // namespace bsgalone::core
