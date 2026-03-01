
#include "AbstractAsyncEventQueue.hh"

namespace messaging {

template<EventTypeLike EventType, EventLike<EventType> Event>
AbstractAsyncEventQueue<EventType, Event>::AbstractAsyncEventQueue(IEventQueuePtr queue)
  : IEventQueue<EventType, Event>()
  , core::CoreObject("async")
  , m_eventQueue(std::move(queue))
{
  addModule("queue");
  setService("event");

  if (nullptr == m_eventQueue)
  {
    throw std::invalid_argument("Expected non null event queue");
  }

  initialize();
}

template<EventTypeLike EventType, EventLike<EventType> Event>
AbstractAsyncEventQueue<EventType, Event>::~AbstractAsyncEventQueue()
{
  if (!m_running.load())
  {
    return;
  }

  m_running.store(false);
  {
    std::unique_lock lock(m_locker);
    m_notifier.notify_one();
  }
  if (m_processingThread.joinable())
  {
    m_processingThread.join();
  }
}

template<EventTypeLike EventType, EventLike<EventType> Event>
void AbstractAsyncEventQueue<EventType, Event>::pushEvent(Base::EventPtr event)
{
  m_eventQueue->pushEvent(std::move(event));
  std::unique_lock lock(m_locker);
  m_notifier.notify_one();
}

template<EventTypeLike EventType, EventLike<EventType> Event>
void AbstractAsyncEventQueue<EventType, Event>::addListener(Base::EventListenerPtr listener)
{
  m_eventQueue->addListener(std::move(listener));
}

template<EventTypeLike EventType, EventLike<EventType> Event>
bool AbstractAsyncEventQueue<EventType, Event>::empty()
{
  return m_eventQueue->empty();
}

template<EventTypeLike EventType, EventLike<EventType> Event>
void AbstractAsyncEventQueue<EventType, Event>::processEvents()
{
  error("Unsupported operation", "Event processing is already asynchronous");
}

template<EventTypeLike EventType, EventLike<EventType> Event>
void AbstractAsyncEventQueue<EventType, Event>::initialize()
{
  m_running.store(true);
  m_processingThread = std::thread(&AbstractAsyncEventQueue<EventType, Event>::asyncEventProcessing,
                                   this);
}

template<EventTypeLike EventType, EventLike<EventType> Event>
void AbstractAsyncEventQueue<EventType, Event>::asyncEventProcessing()
{
  bool running{true};

  debug("Starting event processing");

  while (running)
  {
    // https://en.cppreference.com/w/cpp/thread/condition_variable
    std::unique_lock lock(m_locker);
    m_notifier.wait(lock, [this] { return !m_running.load() || !m_eventQueue->empty(); });

    running = m_running.load();
    if (running)
    {
      m_eventQueue->processEvents();
    }
  }

  debug("Exited event processing");
}

} // namespace messaging
