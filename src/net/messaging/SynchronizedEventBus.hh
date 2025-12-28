
#pragma once

#include "AbstractEventBus.hh"
#include "CoreObject.hh"
#include <deque>
#include <mutex>

namespace net {

class SynchronizedEventBus : public AbstractEventBus, public core::CoreObject
{
  public:
  SynchronizedEventBus();
  ~SynchronizedEventBus() override = default;

  void pushEvent(IEventPtr event) override;
  void addListener(IEventListenerPtr listener) override;
  bool empty() override;

  void processEvents() override;

  private:
  std::mutex m_locker{};
  std::deque<IEventPtr> m_events{};

  void processEvent(const IEvent &event) const;
};

} // namespace net
