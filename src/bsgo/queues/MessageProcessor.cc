
#include "MessageProcessor.hh"

namespace bsgo {

MessageProcessor::MessageProcessor(std::deque<IMessagePtr> &messages,
                                   std::mutex &locker,
                                   MessageHandler handler)
  : CoreObject("processor")
  , m_locker(locker)
  , m_messages(messages)
  , m_handler(handler)
{
  setService("messages");
}

namespace {
const auto UNIMPORTANT_MESSAGE_TYPES = std::unordered_set<MessageType>{MessageType::COMPONENT_SYNC};

struct MessagesInfo
{
  int importantMessagesCount{0};
  int unimportantMessagesCount{0};
  std::string messagesTypes{};
};

auto messagesTypesToString(const std::deque<IMessagePtr> &messages, const int count) -> MessagesInfo
{
  MessagesInfo out{.messagesTypes = "{"};

  for (auto id = 0; id < count; ++id)
  {
    const auto messageType = messages[id]->type();
    if (UNIMPORTANT_MESSAGE_TYPES.contains(messageType))
    {
      ++out.unimportantMessagesCount;
      continue;
    }

    ++out.importantMessagesCount;

    if (id > 0)
    {
      out.messagesTypes += ", ";
    }

    out.messagesTypes += str(messages[id]->type());
  }

  if (out.unimportantMessagesCount > 0)
  {
    // If at least one important message was processed.
    if (out.messagesTypes.size() > 1)
    {
      out.messagesTypes += ", ";
    }
    out.messagesTypes += std::to_string(out.unimportantMessagesCount) + " unimportant message(s)";
  }

  out.messagesTypes += "}";
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
    const auto messagesInfo = messagesTypesToString(messages, count);
    if (messagesInfo.importantMessagesCount > 0)
    {
      info("Processed " + std::to_string(count) + "/" + std::to_string(messages.size())
           + " message(s): " + messagesInfo.messagesTypes);
    }
    else
    {
      verbose("Processed " + std::to_string(count) + "/" + std::to_string(messages.size())
              + " message(s): " + messagesInfo.messagesTypes);
    }
    messages.erase(messages.begin(), messages.begin() + count);
  }

  requeueMessages(std::move(messages));
}

auto MessageProcessor::acquireAndClearMessages() -> std::deque<IMessagePtr>
{
  const std::lock_guard guard(m_locker);
  std::deque<IMessagePtr> messages;
  std::swap(messages, m_messages);
  return messages;
}

void MessageProcessor::requeueMessages(std::deque<IMessagePtr> &&messages)
{
  verbose("Requeuing " + std::to_string(messages.size()) + " message(s)");

  const std::lock_guard guard(m_locker);
  // https://stackoverflow.com/questions/35844999/move-stdvector-to-stddeque-in-c11
  std::move(std::rbegin(messages), std::rend(messages), std::front_inserter(m_messages));
}

} // namespace bsgo
