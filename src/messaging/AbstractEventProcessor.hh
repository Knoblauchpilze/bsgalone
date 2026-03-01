
#pragma once

#include "CoreObject.hh"
#include "EventConcept.hh"
#include "EventTypeConcept.hh"
#include <deque>
#include <functional>
#include <memory>
#include <mutex>
#include <unordered_set>

namespace messaging {

template<EventTypeLike EventType, EventLike<EventType> Event>
class AbstractEventProcessor : public core::CoreObject
{
  public:
  using EventPtr     = std::unique_ptr<Event>;
  using EventHandler = std::function<void(const Event &)>;

  AbstractEventProcessor(const std::string &onBehalfOfName,
                         std::deque<EventPtr> &events,
                         std::mutex &locker,
                         EventHandler handler,
                         std::unordered_set<EventType> eventsToFilterForLogging);

  void processEvents();

  private:
  std::mutex &m_locker;
  std::deque<EventPtr> &m_events;
  EventHandler m_handler{};
  std::unordered_set<EventType> m_loggingFilter{};

  auto acquireAndClearEvents() const -> std::deque<EventPtr>;
  void printEventsInfo(const std::deque<EventPtr> &events) const;

  struct EventsInfo
  {
    int importantEventsCount{0};
    int unimportantEventsCount{0};
    std::string eventsTypes{};
  };
  auto eventsTypesToString(const std::deque<EventPtr> &events) const -> EventsInfo;
};

} // namespace messaging

#include "AbstractEventProcessor.hxx"
