
#include "SynchronizedMessageQueue.hh"

namespace bsgo {

SynchronizedMessageQueue::SynchronizedMessageQueue(IMessageQueuePtr messageQueue)
  : IMessageQueue()
  , utils::CoreObject("synchronized")
  , m_messageQueue(std::move(messageQueue))
{
  addModule("queue");
  setService("message");

  if (nullptr == m_messageQueue)
  {
    throw std::invalid_argument("Expected non null message queue");
  }
}

void SynchronizedMessageQueue::pushMessage(IMessagePtr message)
{
  const std::lock_guard guard(m_locker);
  m_messageQueue->pushMessage(std::move(message));
}

void SynchronizedMessageQueue::addListener(IMessageListenerPtr listener)
{
  const std::lock_guard guard(m_locker);
  m_messageQueue->addListener(std::move(listener));
}

bool SynchronizedMessageQueue::empty()
{
  const std::lock_guard guard(m_locker);
  return m_messageQueue->empty();
}

void SynchronizedMessageQueue::processMessages(const std::optional<int> &amount)
{
  const std::lock_guard guard(m_locker);
  m_messageQueue->processMessages(amount);
}

} // namespace bsgo
