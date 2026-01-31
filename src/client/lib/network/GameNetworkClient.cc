
#include "GameNetworkClient.hh"
#include "AsyncEventBus.hh"
#include "ClientConnectedEvent.hh"
#include "NetworkAdapter.hh"
#include "NetworkMessage.hh"
#include "ServerStartedEvent.hh"
#include "ServerStoppedEvent.hh"
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

  assignClientIdIfPossible(*message);
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

void GameNetworkClient::processMessages(const std::optional<int> &amount)
{
  m_inputQueue->processMessages(amount);
}

namespace {
class NetworkEventListener : public net::IEventListener
{
  public:
  NetworkEventListener(std::atomic_bool &connected, std::atomic<net::ClientId> &clientId)
    : m_connected(connected)
    , m_clientId(clientId)
  {}

  ~NetworkEventListener() = default;

  bool isEventRelevant(const net::EventType &type) const override
  {
    return type == net::EventType::CLIENT_CONNECTED || type == net::EventType::SERVER_STARTED
           || type == net::EventType::SERVER_STOPPED;
  }

  void onEventReceived(const net::IEvent &event) override
  {
    switch (event.type())
    {
      case net::EventType::CLIENT_CONNECTED:
        handleClientConnected(event.as<net::ClientConnectedEvent>());
        break;
      case net::EventType::SERVER_STARTED:
        handleServerStarted(event.as<net::ServerStartedEvent>());
        break;
      case net::EventType::SERVER_STOPPED:
        handleServerStopped(event.as<net::ServerStoppedEvent>());
        break;
      default:
        throw std::invalid_argument("Unsupported event type " + net::str(event.type()));
    }
  }

  private:
  std::atomic_bool &m_connected;
  std::atomic<net::ClientId> &m_clientId;

  void handleClientConnected(const net::ClientConnectedEvent &event)
  {
    m_clientId.store(event.clientId());
  }

  void handleServerStarted(const net::ServerStartedEvent & /*event*/)
  {
    m_connected.store(true);
  }

  void handleServerStopped(const net::ServerStoppedEvent & /*event*/)
  {
    m_connected.store(false);
  }
};
} // namespace

void GameNetworkClient::initialize()
{
  auto networkAdapter = std::make_unique<bsgalone::core::NetworkAdapter>(m_inputQueue);
  m_eventBus->addListener(std::move(networkAdapter));

  auto eventListener = std::make_unique<NetworkEventListener>(m_connected, m_clientId);
  m_eventBus->addListener(std::move(eventListener));
}

void GameNetworkClient::assignClientIdIfPossible(bsgo::IMessage &message) const
{
  if (!message.isA<bsgo::NetworkMessage>())
  {
    throw std::invalid_argument("Unsupported message type " + bsgo::str(message.type()));
  }

  message.as<bsgo::NetworkMessage>().setClientId(m_clientId.load());
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
