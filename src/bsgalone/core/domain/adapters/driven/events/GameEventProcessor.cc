
#include "GameEventProcessor.hh"

namespace bsgalone::core {

GameEventProcessor::GameEventProcessor(const std::string &onBehalfOfName,
                                       std::deque<IGameEventPtr> &events,
                                       std::mutex &locker,
                                       GameEventHandler handler)
  : CoreObject(onBehalfOfName)
  , m_locker(locker)
  , m_events(events)
  , m_handler(handler)
{
  setService("event");
}

void GameEventProcessor::processEvents()
{
  const auto events = acquireAndClearEvents();
  for (const auto &event : events)
  {
    m_handler(*event);
  }

  printEventsInfo(events);
}

auto GameEventProcessor::acquireAndClearEvents() const -> std::deque<IGameEventPtr>
{
  const std::lock_guard guard(m_locker);
  std::deque<IGameEventPtr> events;
  std::swap(events, m_events);
  return events;
}

namespace {
const auto UNIMPORTANT_EVENT_TYPES = std::unordered_set<GameEventType>{};

struct EventsInfo
{
  int importantEventsCount{0};
  int unimportantEventsCount{0};
  std::string eventsTypes{};
};

auto eventsTypesToString(const std::deque<IGameEventPtr> &events) -> EventsInfo
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

void GameEventProcessor::printEventsInfo(const std::deque<IGameEventPtr> &events) const
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

} // namespace bsgalone::core
