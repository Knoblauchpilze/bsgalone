
#include "MessageProcessor.hh"

namespace bsgo {

MessageProcessor::MessageProcessor(std::deque<IMessagePtr> &messages, MessageHandler handler)
  : MessageProcessor(messages, nullptr, handler)
{}

MessageProcessor::MessageProcessor(std::deque<IMessagePtr> &messages,
                                   std::mutex *locker,
                                   MessageHandler handler)
  : CoreObject("processor")
  , m_locker(locker)
  , m_messages(messages)
  , m_handler(handler)
{
  setService("messages");
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

void MessageProcessor::processMessages(const std::optional<int> &amount)
{
  auto messages            = acquireAndClearMessages();
  const auto messagesCount = static_cast<int>(messages.size());

  const auto count = (amount ? std::min(*amount, messagesCount) : messagesCount);
  for (auto id = 0; id < count; ++id)
  {
    const auto &message = messages[id];
    m_handler(*message);
  }

  if (!messages.empty() && count > 0)
  {
    const auto allTypes = messagesTypesToString(messages, count);
    info("Processed " + std::to_string(count) + "/" + std::to_string(messages.size())
         + " message(s): " + allTypes);
    messages.erase(messages.begin(), messages.begin() + count);
  }

  requeueMessages(std::move(messages));
}

auto MessageProcessor::acquireAndClearMessages() -> std::deque<IMessagePtr>
{
  if (m_locker != nullptr)
  {
    const std::lock_guard guard(*m_locker);
    std::deque<IMessagePtr> messages;
    std::swap(messages, m_messages);
    return messages;
  }

  std::deque<IMessagePtr> messages;
  std::swap(messages, m_messages);
  return messages;
}

void MessageProcessor::requeueMessages(std::deque<IMessagePtr> &&messages)
{
  verbose("Requeuing " + std::to_string(messages.size()) + " message(s)");

  if (m_locker != nullptr)
  {
    const std::lock_guard guard(*m_locker);
    // https://stackoverflow.com/questions/35844999/move-stdvector-to-stddeque-in-c11
    std::move(std::rbegin(messages), std::rend(messages), std::front_inserter(m_messages));
    return;
  }

  std::move(std::rbegin(messages), std::rend(messages), std::front_inserter(m_messages));
}

} // namespace bsgo
