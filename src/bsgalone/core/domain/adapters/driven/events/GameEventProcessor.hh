
#pragma once

#include "CoreObject.hh"
#include "IGameEvent.hh"
#include <deque>
#include <functional>
#include <mutex>
#include <optional>

namespace bsgalone::core {

using GameEventHandler = std::function<void(const IGameEvent &)>;

class GameEventProcessor : public ::core::CoreObject
{
  public:
  GameEventProcessor(const std::string &onBehalfOfName,
                     std::deque<IGameEventPtr> &events,
                     std::mutex &locker,
                     GameEventHandler handler);

  void processEvents();

  private:
  std::mutex &m_locker;
  std::deque<IGameEventPtr> &m_events;
  GameEventHandler m_handler{};

  auto acquireAndClearEvents() const -> std::deque<IGameEventPtr>;
  void printEventsInfo(const std::deque<IGameEventPtr> &events) const;
};

} // namespace bsgalone::core
