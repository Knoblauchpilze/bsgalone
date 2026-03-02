
#include "GameNetworkClient.hh"
#include "AsyncEventQueue.hh"
#include "ConnectionEstablishedEvent.hh"
#include "ConnectionLostEvent.hh"
#include "ConnectionMessage.hh"
#include "InputNetworkAdapter.hh"
#include "MessageParser.hh"
#include "OutputNetworkAdapter.hh"
#include "SynchronizedEventQueue.hh"
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
  m_eventBus  = net::createAsyncEventQueue(net::createSynchronizedEventQueue());
  m_tcpClient = std::make_shared<net::TcpClient>(m_eventBus);
  m_adapter   = std::make_unique<bsgalone::core::OutputNetworkAdapter>(m_tcpClient);

  initialize();
  m_tcpClient->connect(DEFAULT_SERVER_URL, port);
}

void GameNetworkClient::stop()
{
  m_tcpClient->disconnect();
  m_adapter.reset();
  m_tcpClient.reset();
  m_eventBus.reset();
}

void GameNetworkClient::pushMessage(bsgalone::core::IMessagePtr message)
{
  if (!m_connected.load())
  {
    throw std::invalid_argument("Failed to send message, not connected to server");
  }

  m_adapter->sendMessage(*message);
}

void GameNetworkClient::addListener(bsgalone::core::IMessageListenerPtr listener)
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
// TODO: Extract this class
class NetworkEventListener : public net::INetworkEventListener
{
  public:
  NetworkEventListener(std::atomic_bool &connected, bsgalone::core::IMessageQueueShPtr inputQueue)
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
  bsgalone::core::IMessageQueueShPtr m_inputQueue{};

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
    = std::make_unique<bsgalone::core::InputNetworkAdapter>(m_inputQueue,
                                                            std::make_unique<bsgo::MessageParser>());
  m_eventBus->addListener(std::move(networkAdapter));

  auto eventListener = std::make_unique<NetworkEventListener>(m_connected, m_inputQueue);
  m_eventBus->addListener(std::move(eventListener));
}

} // namespace pge
