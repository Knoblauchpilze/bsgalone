
#pragma once

#include "CoreObject.hh"
#include <deque>
#include <functional>
#include <memory>
#include <mutex>
#include <unordered_set>

namespace messaging {

// https://stackoverflow.com/questions/60524480/how-to-force-a-template-parameter-to-be-an-enum-or-enum-class
template<typename E>
concept EnumType = std::is_enum_v<E>;

// Template class seems to throw formatting off by formatting as follows:
// public: using ...
// clang-format off
template<class Event, EnumType EventType>
class AbstractEventProcessor : public core::CoreObject {
  public:
  using EventPtr     = std::unique_ptr<Event>;
  using EventHandler = std::function<void(const Event &)>;
  // clang-format on

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

  // clang-format off
  struct EventsInfo{
    int importantEventsCount{0};
    int unimportantEventsCount{0};
    std::string eventsTypes{};
  };
  auto eventsTypesToString(const std::deque<EventPtr> &events) const -> EventsInfo;
};
// clang-format on

} // namespace messaging

#include "AbstractEventProcessor.hxx"
