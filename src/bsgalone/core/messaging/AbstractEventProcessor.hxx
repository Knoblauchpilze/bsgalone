
#pragma once

#include "AbstractEventProcessor.hh"

namespace bsgalone::core {

template<class Event, EnumType EventType>
inline AbstractEventProcessor<Event, EventType>::AbstractEventProcessor(
  const std::string &onBehalfOfName,
  std::deque<EventPtr> &events,
  std::mutex &locker,
  EventHandler handler)
  : ::core::CoreObject(onBehalfOfName)
  , m_locker(locker)
  , m_events(events)
  , m_handler(handler)
{
  setService("event");
}

template<class Event, EnumType EventType>
inline void AbstractEventProcessor<Event, EventType>::processEvents()
{
  const auto events = acquireAndClearEvents();
  for (const auto &event : events)
  {
    m_handler(*event);
  }

  printEventsInfo(events);
}

template<class Event, EnumType EventType>
inline auto AbstractEventProcessor<Event, EventType>::acquireAndClearEvents() const
  -> std::deque<EventPtr>
{
  const std::lock_guard guard(m_locker);
  std::deque<EventPtr> events;
  std::swap(events, m_events);
  return events;
}

// namespace {
// const auto UNIMPORTANT_EVENT_TYPES = std::unordered_set<EventType>{EventType::DATA_RECEIVED,
//                                                                    EventType::DATA_SENT};

// } // namespace

template<class Event, EnumType EventType>
inline void AbstractEventProcessor<Event, EventType>::printEventsInfo(
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

template<class Event, EnumType EventType>
inline auto AbstractEventProcessor<Event, EventType>::eventsTypesToString(
  const std::deque<EventPtr> &events) const -> EventsInfo
{
  EventsInfo out{.eventsTypes = "{"};

  const auto hiddenEventTypes = this->unimportantEventTypes();

  for (const auto &event : events)
  {
    const auto eventType = event->type();
    if (hiddenEventTypes.contains(eventType))
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

} // namespace bsgalone::core
