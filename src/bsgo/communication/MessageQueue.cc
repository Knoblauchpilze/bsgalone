
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

bool MessageQueue::empty()
{
  return m_messages.empty();
}

namespace {
auto messagesTypesToString(const std::vector<IMessagePtr> &messages) -> std::string
{
  std::string out = "{";

  auto id = 0;
  for (const auto &message : messages)
  {
    if (id > 0)
    {
      out += ", ";
    }
    out += str(message->type());
    ++id;
  }

  out += "}";
  return out;
}
} // namespace

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
    const auto allTypes = messagesTypesToString(messages);
    info("Processed " + std::to_string(messages.size()) + " message(s): " + allTypes);
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
