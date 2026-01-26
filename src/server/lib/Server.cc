

#include "Server.hh"
#include "LogoutMessage.hh"
#include "SystemProcessorUtils.hh"

namespace bsgo {

Server::Server()
  : core::CoreObject("server")
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

void Server::initializeMessageSystem()
{
  const MessageSystemData data{.clientManager = m_clientManager, .systemQueues = m_inputQueues};
  m_messageExchanger = std::make_unique<MessageExchanger>(data);

  for (const auto &systemProcessor : m_systemProcessors)
  {
    systemProcessor->connectToQueues(m_messageExchanger->getInternalMessageQueue(),
                                     m_messageExchanger->getOutputMessageQueue());
  }
}

void Server::setup(const int port)
{
  const net::ServerConfig config{.disconnectHandler =
                                   [this](const net::ClientId clientId) {
                                     return onConnectionLost(clientId);
                                   },
                                 .connectionReadyHandler =
                                   [this](net::ConnectionShPtr connection) {
                                     onConnectionReady(connection);
                                   }};

  m_tcpServer = std::make_shared<net::LegacyTcpServer>(m_context, port, config);
  m_tcpServer->start();

  info("Starting listening on port " + std::to_string(m_tcpServer->port()));
  m_context.start();
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
  m_context.stop();
}

void Server::onConnectionLost(const net::ClientId clientId)
{
  if (!m_clientManager->isStillConnected(clientId))
  {
    m_clientManager->removeConnection(clientId);
    return;
  }

  const auto data = m_clientManager->tryGetDataForConnection(clientId);
  if (!data.playerDbId)
  {
    error("Connection " + net::str(clientId) + " lost but could not find associated player");
  }

  if (data.stale)
  {
    debug("Connection " + net::str(clientId) + " is already marked as stale");
    return;
  }

  info("Connection " + net::str(clientId) + " lost but player " + str(*data.playerDbId)
       + " is still connected");

  m_clientManager->markConnectionAsStale(clientId);
  auto message = std::make_unique<LogoutMessage>(*data.playerDbId, true);
  message->setClientId(clientId);

  m_messageExchanger->pushMessage(std::move(message));
}

void Server::onConnectionReady(net::ConnectionShPtr connection)
{
  m_clientManager->registerConnection(connection);
  m_messageExchanger->registerConnection(connection);
}

} // namespace bsgo
