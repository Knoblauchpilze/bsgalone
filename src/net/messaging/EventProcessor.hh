
#pragma once

#include "CoreObject.hh"
#include "IEvent.hh"
#include <deque>
#include <functional>
#include <mutex>
#include <optional>

namespace net {

using EventHandler = std::function<void(const IEvent &)>;

class EventProcessor : public core::CoreObject
{
  public:
  EventProcessor(const std::string &onBehalfOfName,
                 std::deque<IEventPtr> &events,
                 std::mutex &locker,
                 EventHandler handler);

  void processEvents();

  private:
  std::mutex &m_locker;
  std::deque<IEventPtr> &m_events;
  EventHandler m_handler{};

  auto acquireAndClearEvents() const -> std::deque<IEventPtr>;
  void printEventsInfo() const;
};

} // namespace net
