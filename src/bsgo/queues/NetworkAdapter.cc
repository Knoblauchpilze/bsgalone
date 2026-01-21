
#include "NetworkAdapter.hh"
#include "MessageParser.hh"

namespace bsgo {

NetworkAdapter::NetworkAdapter(IMessageQueueShPtr queue)
  : core::CoreObject("adapter")
  , m_queue(std::move(queue))
{
  setService("network");

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
  const auto processed = onDataReceived(dataReceived.clientId());
  removePendingData(dataReceived.clientId(), processed);
}

void NetworkAdapter::registerPendingData(const net::DataReceivedEvent &event)
{
  const std::lock_guard guard(m_locker);

  auto &pendingData = m_pendingData[event.clientId()];
  pendingData.bytes.insert(pendingData.bytes.end(), event.data().begin(), event.data().end());
  debug("pending bytes for " + net::str(event.clientId()) + " = "
        + std::to_string(pendingData.bytes.size()));
}

auto NetworkAdapter::onDataReceived(const net::ClientId clientId) -> int
{
  bool processedSomeBytes{true};
  auto processedBytes{0};
  std::vector<IMessagePtr> messages{};

  std::deque<char> workingData;
  {
    const std::lock_guard guard(m_locker);
    const auto &pendingData = m_pendingData[clientId];
    workingData.insert(workingData.end(), pendingData.bytes.begin(), pendingData.bytes.end());
  }

  MessageParser parser{};

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

  feedMessagesToQueue(std::move(messages));
  return processedBytes;
}

void NetworkAdapter::feedMessagesToQueue(std::vector<IMessagePtr> &&messages)
{
  for (auto &message : messages)
  {
    m_queue->pushMessage(std::move(message));
  }
}

void NetworkAdapter::removePendingData(const net::ClientId clientId, const int processed)
{
  if (processed == 0)
  {
    debug("processed 0 bytes for client " + net::str(clientId));
    return;
  }

  debug("processed " + std::to_string(processed) + " bytes for " + net::str(clientId));

  const std::lock_guard guard(m_locker);
  auto &pendingData = m_pendingData[clientId];
  pendingData.bytes.erase(pendingData.bytes.begin(), pendingData.bytes.begin() + processed);
}

} // namespace bsgo
