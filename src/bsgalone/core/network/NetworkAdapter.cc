
#include "NetworkAdapter.hh"
#include "MessageParser.hh"
#include "NetworkMessage.hh"

namespace bsgalone::core {

NetworkAdapter::NetworkAdapter(bsgo::IMessageQueueShPtr queue)
  : m_queue(std::move(queue))
{
  if (m_queue == nullptr)
  {
    throw std::invalid_argument("Expected non null message queue");
  }
}

bool NetworkAdapter::isEventRelevant(const net::EventType &type) const
{
  return type == net::EventType::DATA_RECEIVED;
}

void NetworkAdapter::onEventReceived(const net::IEvent &event)
{
  const auto &dataReceived = event.as<net::DataReceivedEvent>();
  registerPendingData(dataReceived);
  // TODO: Do not expect this as it might not be provided
  const auto processed = onDataReceived(dataReceived.tryGetClientId().value());
  removePendingData(dataReceived.tryGetClientId().value(), processed);
}

void NetworkAdapter::registerPendingData(const net::DataReceivedEvent &event)
{
  const std::lock_guard guard(m_locker);

  // TODO: Do not expect this as it might not be provided
  auto &pendingData = m_pendingData[event.tryGetClientId().value()];
  std::vector<char> toAdd(event.data());
  pendingData.bytes.insert(pendingData.bytes.end(), toAdd.begin(), toAdd.end());
}

auto NetworkAdapter::onDataReceived(const net::ClientId clientId) -> int
{
  bool processedSomeBytes{true};
  auto processedBytes{0};
  std::vector<bsgo::IMessagePtr> messages{};

  std::deque<char> workingData;
  {
    const std::lock_guard guard(m_locker);
    const auto &pendingData = m_pendingData[clientId];
    workingData.insert(workingData.end(), pendingData.bytes.begin(), pendingData.bytes.end());
  }

  bsgo::MessageParser parser{};

  while (processedSomeBytes)
  {
    auto result = parser.tryParseMessage(workingData);
    if (result.message)
    {
      messages.emplace_back(std::move(*result.message));
    }

    processedSomeBytes = (result.bytesProcessed > 0);
    processedBytes += result.bytesProcessed;

    workingData.erase(workingData.begin(), workingData.begin() + result.bytesProcessed);
  }

  feedMessagesToQueue(clientId, std::move(messages));
  return processedBytes;
}

void NetworkAdapter::feedMessagesToQueue(const net::ClientId clientId,
                                         std::vector<bsgo::IMessagePtr> &&messages)
{
  for (auto &message : messages)
  {
    if (message->isA<bsgo::NetworkMessage>())
    {
      message->as<bsgo::NetworkMessage>().setClientId(clientId);
    }
    m_queue->pushMessage(std::move(message));
  }
}

void NetworkAdapter::removePendingData(const net::ClientId clientId, const int processed)
{
  if (processed == 0)
  {
    return;
  }

  const std::lock_guard guard(m_locker);
  auto &pendingData = m_pendingData[clientId];
  pendingData.bytes.erase(pendingData.bytes.begin(), pendingData.bytes.begin() + processed);
}

} // namespace bsgalone::core
