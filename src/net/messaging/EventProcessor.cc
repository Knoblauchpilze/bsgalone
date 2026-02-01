
#include "EventProcessor.hh"

namespace net {

EventProcessor::EventProcessor(const std::string &onBehalfOfName,
                               std::deque<IEventPtr> &events,
                               std::mutex &locker,
                               EventHandler handler)
  : CoreObject(onBehalfOfName)
  , m_locker(locker)
  , m_events(events)
  , m_handler(handler)
{
  setService("event");
}

void EventProcessor::processEvents()
{
  const auto events = acquireAndClearEvents();
  for (const auto &event : events)
  {
    m_handler(*event);
  }

  printEventsInfo(events);
}

auto EventProcessor::acquireAndClearEvents() const -> std::deque<IEventPtr>
{
  const std::lock_guard guard(m_locker);
  std::deque<IEventPtr> events;
  std::swap(events, m_events);
  return events;
}

namespace {
const auto UNIMPORTANT_EVENT_TYPES = std::unordered_set<EventType>{EventType::DATA_RECEIVED,
                                                                   EventType::DATA_SENT};

struct EventsInfo
{
  int importantEventsCount{0};
  int unimportantEventsCount{0};
  std::string eventsTypes{};
};

auto eventsTypesToString(const std::deque<IEventPtr> &events) -> EventsInfo
{
  EventsInfo out{.eventsTypes = "{"};

  for (const auto &event : events)
  {
    const auto eventType = event->type();
    if (UNIMPORTANT_EVENT_TYPES.contains(eventType))
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
} // namespace

void EventProcessor::printEventsInfo(const std::deque<IEventPtr> &events) const
{
  if (events.empty())
  {
    return;
  }

  const auto eventsInfo = eventsTypesToString(events);
  if (eventsInfo.importantEventsCount > 0)
  {
    info("Processed " + std::to_string(events.size()) + " message(s): " + eventsInfo.eventsTypes);
  }
  else
  {
    verbose("Processed " + std::to_string(events.size()) + " message(s): " + eventsInfo.eventsTypes);
  }
}

} // namespace net
