
#pragma once

#include "AbstractEventProcessor.hh"

namespace messaging {

template<EventTypeLike EventType, EventLike<EventType> Event>
inline AbstractEventProcessor<EventType, Event>::AbstractEventProcessor(
  const std::string &onBehalfOfName,
  std::deque<EventPtr> &events,
  std::mutex &locker,
  EventHandler handler,
  std::unordered_set<EventType> eventsToFilterForLogging)
  : core::CoreObject(onBehalfOfName)
  , m_locker(locker)
  , m_events(events)
  , m_handler(std::move(handler))
  , m_loggingFilter(std::move(eventsToFilterForLogging))
{
  setService("event");
}

template<EventTypeLike EventType, EventLike<EventType> Event>
inline void AbstractEventProcessor<EventType, Event>::processEvents()
{
  const auto events = acquireAndClearEvents();
  for (const auto &event : events)
  {
    m_handler(*event);
  }

  printEventsInfo(events);
}

template<EventTypeLike EventType, EventLike<EventType> Event>
inline auto AbstractEventProcessor<EventType, Event>::acquireAndClearEvents() const
  -> std::deque<EventPtr>
{
  const std::lock_guard guard(m_locker);
  std::deque<EventPtr> events;
  std::swap(events, m_events);
  return events;
}

template<EventTypeLike EventType, EventLike<EventType> Event>
inline void AbstractEventProcessor<EventType, Event>::printEventsInfo(
  const std::deque<EventPtr> &events) const
{
  if (events.empty())
  {
    return;
  }

  const auto eventsInfo = eventsTypesToString(events);
  if (eventsInfo.importantEventsCount > 0)
  {
    info("Processed " + std::to_string(events.size()) + " event(s): " + eventsInfo.eventsTypes);
  }
  else
  {
    verbose("Processed " + std::to_string(events.size()) + " event(s): " + eventsInfo.eventsTypes);
  }
}

template<EventTypeLike EventType, EventLike<EventType> Event>
inline auto AbstractEventProcessor<EventType, Event>::eventsTypesToString(
  const std::deque<EventPtr> &events) const -> EventsInfo
{
  EventsInfo out{.eventsTypes = "{"};

  for (const auto &event : events)
  {
    const auto eventType = event->type();
    if (m_loggingFilter.contains(eventType))
    {
      ++out.unimportantEventsCount;
      continue;
    }

    if (out.importantEventsCount > 0)
    {
      out.eventsTypes += ", ";
    }

    ++out.importantEventsCount;

    out.eventsTypes += str(eventType);
  }

  if (out.unimportantEventsCount > 0)
  {
    // If at least one important event was processed.
    if (out.eventsTypes.size() > 1)
    {
      out.eventsTypes += ", ";
    }
    out.eventsTypes += std::to_string(out.unimportantEventsCount) + " unimportant event(s)";
  }

  out.eventsTypes += "}";
  return out;
}

} // namespace messaging
