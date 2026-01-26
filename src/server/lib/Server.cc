

#include "Server.hh"
#include "AsyncEventBus.hh"
#include "ClientConnectedEvent.hh"
#include "ClientDisconnectedEvent.hh"
#include "ConnectionMessage.hh"
#include "IEventListener.hh"
#include "LogoutMessage.hh"
#include "NetworkAdapter.hh"
#include "SynchronizedEventBus.hh"
#include "SystemProcessorUtils.hh"
#include "TcpServer.hh"

namespace bsgo {

Server::Server()
  : core::CoreObject("server")
  , m_eventBus(std::make_shared<net::AsyncEventBus>(std::make_unique<net::SynchronizedEventBus>()))
  , m_tcpServer(std::make_shared<net::TcpServer>(m_eventBus))
{
  setService("server");
  initialize();
}

void Server::run(const int port)
{
  debug("Starting server...");
  setup(port);

  activeRunLoop();

  debug("Shutting down server...");
  shutdown();
}

void Server::requestStop()
{
  m_running.store(false);
  std::unique_lock lock(m_runningLocker);
  m_runningNotifier.notify_one();
}

void Server::initialize()
{
  initializeSystems();
  initializeMessageSystem();
}

void Server::initializeSystems()
{
  const Repositories repositories;

  const auto allSystems = repositories.systemRepository->findAll();

  for (const auto &systemDbId : allSystems)
  {
    auto inputQueue = std::make_shared<SynchronizedMessageQueue>("synchronized-message-queue-for-"
                                                                 + std::to_string(systemDbId));

    auto processor            = std::make_shared<SystemProcessor>(systemDbId, inputQueue);
    m_inputQueues[systemDbId] = inputQueue;
    m_systemProcessors.emplace_back(std::move(processor));
  }
}

namespace {
class ClientManagerProxy : public net::IEventListener
{
  public:
  ClientManagerProxy(ClientManagerShPtr manager,
                     IMessageQueueShPtr inputQueue,
                     IMessageQueue *outputQueue)
    : m_manager(std::move(manager))
    , m_inputQueue(std::move(inputQueue))
    , m_outputQueue(outputQueue)
  {}

  ~ClientManagerProxy() override = default;

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
  IMessageQueue *m_outputQueue{};

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

void Server::initializeMessageSystem()
{
  const MessageSystemData data{.clientManager = m_clientManager,
                               .server        = m_tcpServer,
                               .systemQueues  = m_inputQueues};
  m_messageExchanger = std::make_unique<MessageExchanger>(data);

  auto adapter = std::make_unique<NetworkAdapter>(m_messageExchanger->getInputMessageQueue());
  m_eventBus->addListener(std::move(adapter));
  m_eventBus->addListener(
    std::make_unique<ClientManagerProxy>(m_clientManager,
                                         m_messageExchanger->getInputMessageQueue(),
                                         m_messageExchanger->getOutputMessageQueue()));

  for (const auto &systemProcessor : m_systemProcessors)
  {
    systemProcessor->connectToQueues(m_messageExchanger->getInternalMessageQueue(),
                                     m_messageExchanger->getOutputMessageQueue());
  }
}

void Server::setup(const int port)
{
  info("Starting listening on port " + std::to_string(port));
  m_tcpServer->start(port);
}

void Server::activeRunLoop()
{
  m_running.store(true);
  bool running{true};

  for (const auto &processor : m_systemProcessors)
  {
    processor->start();
  }

  while (running)
  {
    std::unique_lock lock(m_runningLocker);
    m_runningNotifier.wait(lock, [this] { return !m_running.load(); });

    running = m_running.load();
  }

  for (const auto &systemProcessor : m_systemProcessors)
  {
    systemProcessor->stop();
  }
}

void Server::shutdown()
{
  m_tcpServer->stop();
}

} // namespace bsgo
