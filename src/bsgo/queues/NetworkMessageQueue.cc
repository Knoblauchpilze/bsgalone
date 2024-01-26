
#include "NetworkMessageQueue.hh"
#include "MessageParser.hh"
#include "SynchronizedMessageQueue.hh"

namespace bsgo {

NetworkMessageQueue::NetworkMessageQueue(IMessageQueuePtr synchronizedQueue)
  : IMessageQueue()
  , utils::CoreObject("message")
  , m_synchronizedQueue(std::move(synchronizedQueue))
{
  addModule("queue");
  setService("network");
}

void NetworkMessageQueue::registerToConnection(net::Connection &connection)
{
  connection.setDataHandler(
    [this](const net::ConnectionId connectionId, const std::deque<char> &data) {
      return onDataReceived(connectionId, data);
    });
}

void NetworkMessageQueue::pushMessage(IMessagePtr message)
{
  m_synchronizedQueue->pushMessage(std::move(message));
}

void NetworkMessageQueue::addListener(IMessageListenerPtr listener)
{
  m_synchronizedQueue->addListener(std::move(listener));
}

bool NetworkMessageQueue::empty()
{
  return m_synchronizedQueue->empty();
}

void NetworkMessageQueue::processMessages(const std::optional<int> &amount)
{
  m_synchronizedQueue->processMessages(amount);
}

auto NetworkMessageQueue::onDataReceived(const net::ConnectionId /*connectionId*/,
                                         const std::deque<char> &data) -> int
{
  bool processedSomeBytes{true};
  auto processedBytes{0};
  std::vector<IMessagePtr> messages{};

  std::deque<char> workingData(data);
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

void NetworkMessageQueue::feedMessagesToQueue(std::vector<IMessagePtr> &&messages)
{
  for (auto &message : messages)
  {
    m_synchronizedQueue->pushMessage(std::move(message));
  }
}

} // namespace bsgo
