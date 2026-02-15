
#include "InputNetworkAdapter.hh"
#include "NetworkMessage.hh"

namespace bsgalone::core {

InputNetworkAdapter::InputNetworkAdapter(IMessageQueueShPtr queue, IMessageParserPtr parser)
  : m_queue(std::move(queue))
  , m_parser(std::move(parser))
{
  if (m_queue == nullptr)
  {
    throw std::invalid_argument("Expected non null message queue");
  }
  if (m_parser == nullptr)
  {
    throw std::invalid_argument("Expected non null message parser");
  }
}

bool InputNetworkAdapter::isEventRelevant(const net::EventType &type) const
{
  return type == net::EventType::DATA_RECEIVED;
}

void InputNetworkAdapter::onEventReceived(const net::IEvent &event)
{
  const auto &dataReceived = event.as<net::DataReceivedEvent>();
  registerPendingData(dataReceived);
  const auto processed = onDataReceived(dataReceived.tryGetClientId());
  removePendingData(dataReceived.tryGetClientId(), processed);
}

void InputNetworkAdapter::registerPendingData(const net::DataReceivedEvent &event)
{
  const std::lock_guard guard(m_locker);

  std::vector<char> toAdd(event.data());

  const auto maybeClientId = event.tryGetClientId();
  if (!maybeClientId)
  {
    m_noClientData.bytes.insert(m_noClientData.bytes.end(), toAdd.begin(), toAdd.end());
  }
  else
  {
    auto &pendingData = m_pendingData[*maybeClientId];
    pendingData.bytes.insert(pendingData.bytes.end(), toAdd.begin(), toAdd.end());
  }
}

auto InputNetworkAdapter::onDataReceived(const std::optional<net::ClientId> &maybeClientId) -> int
{
  bool processedSomeBytes{true};
  auto processedBytes{0};
  std::vector<IMessagePtr> messages{};

  std::deque<char> workingData;
  {
    const std::lock_guard guard(m_locker);
    if (!maybeClientId)
    {
      workingData.insert(workingData.end(),
                         m_noClientData.bytes.begin(),
                         m_noClientData.bytes.end());
    }
    else
    {
      const auto &pendingData = m_pendingData[*maybeClientId];
      workingData.insert(workingData.end(), pendingData.bytes.begin(), pendingData.bytes.end());
    }
  }

  while (processedSomeBytes)
  {
    auto result = m_parser->tryParseMessage(workingData);
    if (result.message)
    {
      messages.emplace_back(std::move(*result.message));
    }

    processedSomeBytes = (result.bytesProcessed > 0);
    processedBytes += result.bytesProcessed;

    workingData.erase(workingData.begin(), workingData.begin() + result.bytesProcessed);
  }

  feedMessagesToQueue(maybeClientId, std::move(messages));
  return processedBytes;
}

void InputNetworkAdapter::feedMessagesToQueue(const std::optional<net::ClientId> &maybeClientId,
                                              std::vector<IMessagePtr> &&messages)
{
  for (auto &message : messages)
  {
    if (maybeClientId && message->isA<NetworkMessage>())
    {
      message->as<NetworkMessage>().setClientId(*maybeClientId);
    }
    m_queue->pushMessage(std::move(message));
  }
}

void InputNetworkAdapter::removePendingData(const std::optional<net::ClientId> &maybeClientId,
                                            const int processed)
{
  if (processed == 0)
  {
    return;
  }

  const std::lock_guard guard(m_locker);
  if (!maybeClientId)
  {
    m_noClientData.bytes.erase(m_noClientData.bytes.begin(),
                               m_noClientData.bytes.begin() + processed);
  }
  else
  {
    auto &pendingData = m_pendingData[*maybeClientId];
    pendingData.bytes.erase(pendingData.bytes.begin(), pendingData.bytes.begin() + processed);
  }
}

} // namespace bsgalone::core
