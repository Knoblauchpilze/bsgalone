
#include "SynchronizedMessageQueue.hh"
#include "MessageProcessor.hh"

#include <iostream>

namespace bsgo {

SynchronizedMessageQueue::SynchronizedMessageQueue(const std::string &name)
  : AbstractMessageQueue()
  , utils::CoreObject(name)
{
  setService("message");
  addModule("queue");
}

void SynchronizedMessageQueue::pushMessage(IMessagePtr message)
{
  const std::lock_guard guard(m_locker);
  m_messages.emplace_back(std::move(message));
}

void SynchronizedMessageQueue::addListener(IMessageListenerPtr listener)
{
  const std::lock_guard guard(m_locker);
  this->AbstractMessageQueue::addListener(std::move(listener));
}

bool SynchronizedMessageQueue::empty()
{
  const std::lock_guard guard(m_locker);
  return m_messages.empty();
}

void SynchronizedMessageQueue::processMessages(const std::optional<int> &amount)
{
  std::cout << getName() << " processing messages" << std::endl;
  MessageProcessor processor(m_messages, m_locker, [this](const IMessage &message) {
    processMessage(message);
  });

  processor.processMessages(getName(), amount);
  std::cout << getName() << " processed messages" << std::endl;
}

void SynchronizedMessageQueue::processMessage(const IMessage &message) const
{
  // https://stackoverflow.com/questions/72841621/finding-all-the-values-with-given-key-for-multimap
  auto [it, end] = m_listenersTable.equal_range(message.type());
  for (; it != end; ++it)
  {
    it->second->onMessageReceived(message);
  }
}

} // namespace bsgo
