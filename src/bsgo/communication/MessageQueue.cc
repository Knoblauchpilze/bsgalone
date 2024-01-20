
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
auto messagesTypesToString(const std::deque<IMessagePtr> &messages, const int count) -> std::string
{
  std::string out = "{";

  for (auto id = 0; id < count; ++id)
  {
    if (id > 0)
    {
      out += ", ";
    }
    out += str(messages[id]->type());
    ++id;
  }

  out += "}";
  return out;
}
} // namespace

void MessageQueue::processMessages(const std::optional<int> &amount)
{
  std::deque<IMessagePtr> messages;
  std::swap(messages, m_messages);
  const auto messagesCount = static_cast<int>(messages.size());

  auto count = (amount ? *amount : messagesCount);
  for (auto id = 0; id < count; ++id)
  {
    const auto &message = messages[id];
    processMessage(*message);
  }

  if (!messages.empty() && count > 0)
  {
    const auto allTypes = messagesTypesToString(messages, count);
    info("Processed " + std::to_string(count) + "/" + std::to_string(messages.size())
         + " message(s): " + allTypes);
  }

  if (count < messagesCount)
  {
    for (auto id = count; id < messagesCount; ++id)
    {
      m_messages.emplace_front(std::move(messages[id]));
    }

    verbose("requeued " + std::to_string(messages.size() - count) + " message(s)");
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
