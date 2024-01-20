
#include "AsyncMessageQueue.hh"

namespace bsgo {

AsyncMessageQueue::AsyncMessageQueue(IMessageQueuePtr messageQueue)
  : utils::CoreObject("async")
  , m_messageQueue(std::move(messageQueue))
{
  addModule("queue");
  setService("message");

  if (nullptr == m_messageQueue)
  {
    throw std::invalid_argument("Expected non null message queue");
  }

  initialize();
}

void AsyncMessageQueue::pushMessage(IMessagePtr message)
{
  m_messageQueue->pushMessage(std::move(message));
  m_messageNotifier.notify_one();
}

void AsyncMessageQueue::addListener(IMessageListener *listener)
{
  m_messageQueue->addListener(listener);
}

bool AsyncMessageQueue::empty()
{
  return m_messageQueue->empty();
}

void AsyncMessageQueue::processMessages()
{
  m_messageQueue->processMessages();
}

AsyncMessageQueue::~AsyncMessageQueue()
{
  if (!m_running.load())
  {
    return;
  }

  m_running.store(false);
  m_messageNotifier.notify_one();
  if (m_queueThread.joinable())
  {
    m_queueThread.join();
  }
}

void AsyncMessageQueue::initialize()
{
  m_running.store(true);
  m_queueThread = std::thread(&AsyncMessageQueue::asyncMessageProcessing, this);
}

void AsyncMessageQueue::asyncMessageProcessing()
{
  bool running{true};

  while (running)
  {
    // https://en.cppreference.com/w/cpp/thread/condition_variable
    std::unique_lock lock(m_messageLocker);
    m_messageNotifier.wait(lock, [this] { return !m_running.load() || !m_messageQueue->empty(); });

    running = m_running.load();
    if (running)
    {
      processMessages();
    }
  }

  debug("Exited message processing");
}

} // namespace bsgo
