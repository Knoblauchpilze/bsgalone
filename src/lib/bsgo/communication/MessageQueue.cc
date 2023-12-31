
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

void MessageQueue::addListener(IMessageListener *listener)
{
  if (nullptr == listener)
  {
    error("Failed to add listener", "Null listener");
  }

  for (const auto &messageType : allMessageTypes())
  {
    if (listener->isMessageRelevant(messageType))
    {
      m_listeners.emplace(messageType, listener);
    }
  }
}

void MessageQueue::processMessages()
{
  std::vector<IMessagePtr> messages;
  std::swap(messages, m_messages);

  for (const auto &message : messages)
  {
    processMessage(*message);
  }

  if (!messages.empty())
  {
    info("Processed " + std::to_string(messages.size()) + " message(s)");
  }
}

void MessageQueue::processMessage(const IMessage &message) const
{
  // https://stackoverflow.com/questions/72841621/finding-all-the-values-with-given-key-for-multimap
  auto [it, end] = m_listeners.equal_range(message.type());
  for (; it != end; ++it)
  {
    it->second->onMessageReceived(message);
  }
}

} // namespace bsgo
