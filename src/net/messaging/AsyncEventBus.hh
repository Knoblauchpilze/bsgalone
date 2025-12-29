
#pragma once

#include "CoreObject.hh"
#include "IEventBus.hh"
#include <atomic>
#include <condition_variable>
#include <thread>

namespace net {

class AsyncEventBus : public IEventBus, public core::CoreObject
{
  public:
  AsyncEventBus(IEventBusPtr eventBus);
  ~AsyncEventBus() override;

  void pushEvent(IEventPtr event) override;
  void addListener(IEventListenerPtr listener) override;
  bool empty() override;

  void processEvents() override;

  private:
  IEventBusPtr m_eventBus{};

  std::atomic_bool m_running{false};
  std::thread m_processingThread{};

  std::mutex m_locker{};
  std::condition_variable m_notifier{};

  void initialize();
  void asyncEventProcessing();
};

} // namespace net
