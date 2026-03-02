
#pragma once

#include "CoreObject.hh"
#include "EventConcept.hh"
#include "EventTypeConcept.hh"
#include "IEventQueue.hh"
#include <atomic>
#include <condition_variable>
#include <thread>

namespace messaging {

template<EventTypeLike EventType, EventLike<EventType> Event>
class AbstractAsyncEventQueue : public IEventQueue<EventType, Event>, public core::CoreObject
{
  public:
  using Base           = IEventQueue<EventType, Event>;
  using IEventQueuePtr = std::unique_ptr<Base>;

  AbstractAsyncEventQueue(IEventQueuePtr queue);
  ~AbstractAsyncEventQueue() override;

  void pushEvent(Base::EventPtr event) override;
  void addListener(Base::EventListenerPtr listener) override;
  bool empty() override;

  void processEvents() override;

  private:
  IEventQueuePtr m_eventQueue{};

  std::atomic_bool m_running{false};
  std::thread m_processingThread{};

  std::mutex m_locker{};
  std::condition_variable m_notifier{};

  void initialize();
  void asyncEventProcessing();
};

} // namespace messaging

#include "AbstractAsyncEventQueue.hxx"
