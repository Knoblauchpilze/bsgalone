
#include "GameNetworkClient.hh"
#include "AsyncEventBus.hh"
#include "ConnectionEstablishedEvent.hh"
#include "ConnectionLostEvent.hh"
#include "ConnectionMessage.hh"
#include "MessageParser.hh"
#include "NetworkAdapter.hh"
#include "SynchronizedEventBus.hh"
#include "SynchronizedMessageQueue.hh"
#include "TcpClient.hh"

namespace pge {
constexpr auto DEFAULT_SERVER_URL = "127.0.0.1";

GameNetworkClient::GameNetworkClient()
  : m_inputQueue(
    std::make_shared<bsgo::SynchronizedMessageQueue>("synchronized-message-queue-for-input"))
{}

void GameNetworkClient::start(const int port)
{
  m_eventBus  = std::make_shared<net::AsyncEventBus>(std::make_unique<net::SynchronizedEventBus>());
  m_tcpClient = std::make_shared<net::TcpClient>(m_eventBus);

  initialize();
  m_tcpClient->connect(DEFAULT_SERVER_URL, port);
}

void GameNetworkClient::stop()
{
  m_tcpClient->disconnect();
  m_tcpClient.reset();
  m_eventBus.reset();
}

void GameNetworkClient::pushMessage(bsgo::IMessagePtr message)
{
  if (!m_connected.load())
  {
    throw std::invalid_argument("Failed to send message, not connected to server");
  }

  sendMessage(*message);
}

void GameNetworkClient::addListener(bsgo::IMessageListenerPtr listener)
{
  m_inputQueue->addListener(std::move(listener));
}

bool GameNetworkClient::empty()
{
  return m_inputQueue->empty();
}

void GameNetworkClient::processMessages()
{
  m_inputQueue->processMessages();
}

namespace {
class NetworkEventListener : public net::IEventListener
{
  public:
  NetworkEventListener(std::atomic_bool &connected, bsgo::IMessageQueueShPtr inputQueue)
    : m_connected(connected)
    , m_inputQueue(std::move(inputQueue))
  {}

  ~NetworkEventListener() = default;

  bool isEventRelevant(const net::EventType &type) const override
  {
    return type == net::EventType::CONNECTION_ESTABLISHED
           || type == net::EventType::CONNECTION_LOST;
  }

  void onEventReceived(const net::IEvent &event) override
  {
    switch (event.type())
    {
      case net::EventType::CONNECTION_ESTABLISHED:
        handleConnectionEstablished(event.as<net::ConnectionEstablishedEvent>());
        break;
      case net::EventType::CONNECTION_LOST:
        handleConnectionLost(event.as<net::ConnectionLostEvent>());
        break;
      default:
        throw std::invalid_argument("Unsupported event type " + net::str(event.type()));
    }
  }

  private:
  std::atomic_bool &m_connected;
  bsgo::IMessageQueueShPtr m_inputQueue{};

  void handleConnectionEstablished(const net::ConnectionEstablishedEvent & /*event*/)
  {
    m_connected.store(true);

    auto message = std::make_unique<bsgo::ConnectionMessage>();
    m_inputQueue->pushMessage(std::move(message));
  }

  void handleConnectionLost(const net::ConnectionLostEvent & /*event*/)
  {
    m_connected.store(false);
  }
};
} // namespace

void GameNetworkClient::initialize()
{
  auto networkAdapter
    = std::make_unique<bsgalone::core::NetworkAdapter>(m_inputQueue,
                                                       std::make_unique<bsgo::MessageParser>());
  m_eventBus->addListener(std::move(networkAdapter));

  auto eventListener = std::make_unique<NetworkEventListener>(m_connected, m_inputQueue);
  m_eventBus->addListener(std::move(eventListener));
}

void GameNetworkClient::sendMessage(bsgo::IMessage &message) const
{
  std::ostringstream out{};
  out << message;

  const auto &rawMessage = out.str();
  std::vector<char> data(rawMessage.begin(), rawMessage.end());

  m_tcpClient->trySend(data);
}

} // namespace pge
