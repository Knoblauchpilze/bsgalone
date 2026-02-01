
#include "MessageProcessor.hh"

namespace bsgo {

MessageProcessor::MessageProcessor(const std::string &onBehalfOfName,
                                   std::deque<IMessagePtr> &messages,
                                   std::mutex &locker,
                                   MessageHandler handler)
  : CoreObject(onBehalfOfName)
  , m_locker(locker)
  , m_messages(messages)
  , m_handler(handler)
{
  setService("message");
}

void MessageProcessor::processMessages()
{
  const auto messages = acquireAndClearMessages();
  for (const auto &message : messages)
  {
    m_handler(*message);
  }

  printMessagesInfo(messages);
}

auto MessageProcessor::acquireAndClearMessages() -> std::deque<IMessagePtr>
{
  const std::lock_guard guard(m_locker);
  std::deque<IMessagePtr> messages;
  std::swap(messages, m_messages);
  return messages;
}

namespace {
const auto UNIMPORTANT_MESSAGE_TYPES = std::unordered_set<MessageType>{MessageType::COMPONENT_SYNC};

struct MessagesInfo
{
  int importantMessagesCount{0};
  int unimportantMessagesCount{0};
  std::string messagesTypes{};
};

auto messagesTypesToString(const std::deque<IMessagePtr> &messages) -> MessagesInfo
{
  MessagesInfo out{.messagesTypes = "{"};

  for (const auto &message : messages)
  {
    const auto messageType = message->type();
    if (UNIMPORTANT_MESSAGE_TYPES.contains(messageType))
    {
      ++out.unimportantMessagesCount;
      continue;
    }

    if (out.importantMessagesCount > 0)
    {
      out.messagesTypes += ", ";
    }

    ++out.importantMessagesCount;

    out.messagesTypes += str(messageType);
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

void MessageProcessor::printMessagesInfo(const std::deque<IMessagePtr> &messages) const
{
  if (messages.empty())
  {
    return;
  }

  const auto messagesInfo = messagesTypesToString(messages);
  if (messagesInfo.importantMessagesCount > 0)
  {
    info("Processed " + std::to_string(messages.size())
         + " message(s): " + messagesInfo.messagesTypes);
  }
  else
  {
    verbose("Processed " + std::to_string(messages.size())
            + " message(s): " + messagesInfo.messagesTypes);
  }
}

} // namespace bsgo
