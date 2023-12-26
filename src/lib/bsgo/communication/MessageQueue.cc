
#include "MessageQueue.hh"

namespace bsgo {

MessageQueue::MessageQueue()
  : IMessageQueue()
  , utils::CoreObject("queue")
{
  setService("message");
}

void MessageQueue::pushMessage(IMessagePtr message)
{
  m_messages.emplace_back(std::move(message));
}

} // namespace bsgo
