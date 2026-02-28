
#pragma once

#include "AbstractGameEventQueue.hh"
#include "CoreObject.hh"
#include <deque>
#include <mutex>

namespace bsgalone::core {

class SynchronizedGameEventQueue : public AbstractGameEventQueue, public ::core::CoreObject
{
  public:
  SynchronizedGameEventQueue();
  ~SynchronizedGameEventQueue() override = default;

  void publishEvent(IGameEventPtr event) override;

  void addListener(IGameEventListenerPtr listener) override;
  bool empty() override;
  void processEvents() override;

  private:
  std::mutex m_locker{};
  std::deque<IGameEventPtr> m_events{};

  void processEvent(const IGameEvent &event) const;
};

} // namespace bsgalone::core
