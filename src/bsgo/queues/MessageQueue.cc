
#include "MessageQueue.hh"
#include "MessageProcessor.hh"

namespace bsgo {

MessageQueue::MessageQueue()
  : AbstractMessageQueue()
  , utils::CoreObject("queue")
{
  setService("message");
}

void MessageQueue::pushMessage(IMessagePtr message)
{
  m_messages.emplace_back(std::move(message));
}

bool MessageQueue::empty()
{
  return m_messages.empty();
}

void MessageQueue::processMessages(const std::optional<int> &amount)
{
  MessageProcessor processor(m_messages,
                             [this](const IMessage &message) { processMessage(message); });

  processor.processMessages(amount);
}

void MessageQueue::processMessage(const IMessage &message) const
{
  // https://stackoverflow.com/questions/72841621/finding-all-the-values-with-given-key-for-multimap
  auto [it, end] = m_listenersTable.equal_range(message.type());
  for (; it != end; ++it)
  {
    it->second->onMessageReceived(message);
  }
}

} // namespace bsgo
