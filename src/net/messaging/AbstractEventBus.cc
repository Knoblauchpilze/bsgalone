
#include "AbstractEventBus.hh"

namespace net {

void AbstractEventBus::addListener(IEventListenerPtr listener)
{
  if (nullptr == listener)
  {
    throw std::invalid_argument("Expected a non null listener");
  }

  for (const auto &eventType : allEventTypesAsSet())
  {
    if (listener->isEventRelevant(eventType))
    {
      m_listenersTable.emplace(eventType, listener.get());
    }
  }

  m_listeners.emplace_back(std::move(listener));
}

} // namespace net
