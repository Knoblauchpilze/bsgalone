
#include "ServerNetworkClient.hh"
#include "AsyncEventBus.hh"
#include "AsyncMessageQueue.hh"
#include "BroadcastMessageQueue.hh"
#include "ClientConnectedEvent.hh"
#include "ClientDisconnectedEvent.hh"
#include "ConnectionMessage.hh"
#include "LogoutMessage.hh"
#include "NetworkAdapter.hh"
#include "NetworkMessage.hh"
#include "ServerStartedEvent.hh"
#include "ServerStoppedEvent.hh"
#include "SynchronizedEventBus.hh"
#include "SynchronizedMessageQueue.hh"
#include "TcpServer.hh"

namespace bsgo {

ServerNetworkClient::ServerNetworkClient(ClientManagerShPtr clientManager)
  : m_clientManager(std::move(clientManager))
  , m_inputQueue(std::make_shared<AsyncMessageQueue>(
      std::make_unique<SynchronizedMessageQueue>("synchronized-queue-for-input")))
{}

void ServerNetworkClient::start(const int port)
{
  m_eventBus  = std::make_shared<net::AsyncEventBus>(std::make_unique<net::SynchronizedEventBus>());
  m_tcpServer = std::make_shared<net::TcpServer>(m_eventBus);

  initialize();
  m_tcpServer->start(port);
}

void ServerNetworkClient::stop()
{
  m_tcpServer->stop();
  m_tcpServer.reset();
  m_eventBus.reset();

  m_outputQueue.reset();
}

void ServerNetworkClient::pushMessage(IMessagePtr message)
{
  if (!m_started.load())
  {
    throw std::invalid_argument("Failed to send message, server not started");
  }

  m_outputQueue->pushMessage(std::move(message));
}

void ServerNetworkClient::addListener(IMessageListenerPtr listener)
{
  m_inputQueue->addListener(std::move(listener));
}

bool ServerNetworkClient::empty()
{
  return m_inputQueue->empty();
}

void ServerNetworkClient::processMessages(const std::optional<int> &amount)
{
  m_inputQueue->processMessages(amount);
}

namespace {
class ServerEventListener : public net::IEventListener
{
  public:
  ServerEventListener(std::atomic_bool &started)
    : m_started(started)
  {}

  ~ServerEventListener() = default;

  bool isEventRelevant(const net::EventType &type) const override
  {
    return type == net::EventType::SERVER_STARTED || type == net::EventType::SERVER_STOPPED;
  }

  void onEventReceived(const net::IEvent &event) override
  {
    switch (event.type())
    {
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
  std::atomic_bool &m_started;

  void handleServerStarted(const net::ServerStartedEvent & /*event*/)
  {
    m_started.store(true);
  }

  void handleServerStopped(const net::ServerStoppedEvent & /*event*/)
  {
    m_started.store(false);
  }
};

class ClientEventListener : public net::IEventListener
{
  public:
  ClientEventListener(ClientManagerShPtr clientManager,
                      IMessageQueueShPtr inputQueue,
                      IMessageQueueShPtr outputQueue)
    : m_manager(std::move(clientManager))
    , m_inputQueue(std::move(inputQueue))
    , m_outputQueue(std::move(outputQueue))
  {}

  ~ClientEventListener() = default;

  bool isEventRelevant(const net::EventType &type) const override
  {
    return type == net::EventType::CLIENT_CONNECTED || type == net::EventType::CLIENT_DISCONNECTED;
  }

  void onEventReceived(const net::IEvent &event) override
  {
    switch (event.type())
    {
      case net::EventType::CLIENT_CONNECTED:
        handleClientConnected(event.as<net::ClientConnectedEvent>());
        break;
      case net::EventType::CLIENT_DISCONNECTED:
        handleClientDisconnected(event.as<net::ClientDisconnectedEvent>());
        break;
      default:
        throw std::invalid_argument("Unsupported event type " + net::str(event.type()));
    }
  }

  private:
  ClientManagerShPtr m_manager{};
  IMessageQueueShPtr m_inputQueue{};
  IMessageQueueShPtr m_outputQueue{};

  void handleClientConnected(const net::ClientConnectedEvent &event)
  {
    m_manager->registerClient(event.clientId());

    auto message = std::make_unique<ConnectionMessage>(event.clientId());
    message->validate();
    m_outputQueue->pushMessage(std::move(message));
  }

  void handleClientDisconnected(const net::ClientDisconnectedEvent &event)
  {
    const auto maybePlayerId = m_manager->tryGetPlayerForClient(event.clientId());

    m_manager->removeClient(event.clientId());

    if (!maybePlayerId)
    {
      return;
    }

    auto message = std::make_unique<LogoutMessage>(*maybePlayerId, true);
    message->setClientId(event.clientId());
    m_inputQueue->pushMessage(std::move(message));
  }
};
} // namespace

void ServerNetworkClient::initialize()
{
  auto eventListener = std::make_unique<ServerEventListener>(m_started);
  m_eventBus->addListener(std::move(eventListener));

  auto networkAdapter = std::make_unique<bsgalone::core::NetworkAdapter>(m_inputQueue);
  m_eventBus->addListener(std::move(networkAdapter));

  auto broadcastQueue = std::make_unique<BroadcastMessageQueue>(m_clientManager, m_tcpServer);
  m_outputQueue       = std::make_shared<AsyncMessageQueue>(std::move(broadcastQueue));

  auto clientListener = std::make_unique<ClientEventListener>(m_clientManager,
                                                              m_inputQueue,
                                                              m_outputQueue);
  m_eventBus->addListener(std::move(clientListener));
}

} // namespace bsgo
