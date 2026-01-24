
#include "AsyncEventBus.hh"

namespace net {

AsyncEventBus::AsyncEventBus(IEventBusPtr eventBus)
  : core::CoreObject("async")
  , m_eventBus(std::move(eventBus))
{
  addModule("bus");
  setService("event");

  if (nullptr == m_eventBus)
  {
    throw std::invalid_argument("Expected non null event bus");
  }

  initialize();
}

AsyncEventBus::~AsyncEventBus()
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

void AsyncEventBus::pushEvent(IEventPtr event)
{
  m_eventBus->pushEvent(std::move(event));
  std::unique_lock lock(m_locker);
  m_notifier.notify_one();
}

void AsyncEventBus::addListener(IEventListenerPtr listener)
{
  m_eventBus->addListener(std::move(listener));
}

bool AsyncEventBus::empty()
{
  return m_eventBus->empty();
}

void AsyncEventBus::processEvents()
{
  error("Unsupported operation", "Event processing is already asynchronous");
}

void AsyncEventBus::initialize()
{
  m_running.store(true);
  m_processingThread = std::thread(&AsyncEventBus::asyncEventProcessing, this);
}

void AsyncEventBus::asyncEventProcessing()
{
  bool running{true};

  debug("Starting event processing");

  while (running)
  {
    // https://en.cppreference.com/w/cpp/thread/condition_variable
    std::unique_lock lock(m_locker);
    m_notifier.wait(lock, [this] { return !m_running.load() || !m_eventBus->empty(); });

    running = m_running.load();
    if (running)
    {
      m_eventBus->processEvents();
    }
  }

  debug("Exited event processing");
}

} // namespace net
