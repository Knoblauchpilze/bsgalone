
#include "SynchronizedMessageQueue.hh"

namespace bsgo {

SynchronizedMessageQueue::SynchronizedMessageQueue()
  : utils::CoreObject("synchronized")
{
  addModule("queue");
  setService("message");
}

void SynchronizedMessageQueue::pushMessage(IMessagePtr message)
{
  const std::lock_guard guard(m_locker);
  m_messages.emplace_back(std::move(message));
}

} // namespace bsgo
