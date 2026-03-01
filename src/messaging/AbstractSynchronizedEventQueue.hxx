
#include "AbstractSynchronizedEventQueue.hh"
#include "EventProcessor.hh"

namespace messaging {

template<EventTypeLike EventType, EventLike<EventType> Event>
inline AbstractSynchronizedEventQueue<EventType, Event>::AbstractSynchronizedEventQueue(
  std::unordered_set<EventType> allEvents,
  std::unordered_set<EventType> eventsToFilterForLogging)
  : IEventQueue<EventType, Event>()
  , core::CoreObject("queue")
  , m_allEvents(std::move(allEvents))
  , m_loggingFilter(std::move(eventsToFilterForLogging))
{
  setService("event");
  addModule("synchronized");
}

template<EventTypeLike EventType, EventLike<EventType> Event>
inline void AbstractSynchronizedEventQueue<EventType, Event>::pushEvent(Base::EventPtr event)
{
  const std::lock_guard guard(m_locker);
  m_events.emplace_back(std::move(event));
}

template<EventTypeLike EventType, EventLike<EventType> Event>
inline void AbstractSynchronizedEventQueue<EventType, Event>::addListener(
  Base::EventListenerPtr listener)
{
  if (nullptr == listener)
  {
    throw std::invalid_argument("Expected a non null listener");
  }

  for (const auto &eventType : m_allEvents)
  {
    if (listener->isEventRelevant(eventType))
    {
      m_listenersTable.emplace(eventType, listener.get());
    }
  }

  m_listeners.emplace_back(std::move(listener));
}

template<EventTypeLike EventType, EventLike<EventType> Event>
inline bool AbstractSynchronizedEventQueue<EventType, Event>::empty()
{
  const std::lock_guard guard(m_locker);
  return m_events.empty();
}

template<EventTypeLike EventType, EventLike<EventType> Event>
inline void AbstractSynchronizedEventQueue<EventType, Event>::processEvents()
{
  AbstractEventProcessor<EventType, Event> processor(
    getName(),
    m_events,
    m_locker,
    [this](const Event &event) { processEvent(event); },
    m_loggingFilter);

  processor.processEvents();
}

template<EventTypeLike EventType, EventLike<EventType> Event>
inline void AbstractSynchronizedEventQueue<EventType, Event>::processEvent(const Event &event) const
{
  // https://stackoverflow.com/questions/72841621/finding-all-the-values-with-given-key-for-multimap
  auto [it, end] = m_listenersTable.equal_range(event.type());
  for (; it != end; ++it)
  {
    it->second->onEventReceived(event);
  }
}

} // namespace messaging
