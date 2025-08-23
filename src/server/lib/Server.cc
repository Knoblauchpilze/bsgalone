

#include "Server.hh"
#include "LogoutMessage.hh"
#include "SystemProcessorUtils.hh"
#include "TimeUtils.hh"

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
    auto processor = std::make_shared<SystemProcessor>(systemDbId);
    m_systemProcessors.emplace_back(std::move(processor));
  }
}

void Server::initializeMessageSystem()
{
  const MessageSystemData data{.clientManager    = m_clientManager,
                               .systemProcessors = convertToSystemProcessorMap(m_systemProcessors)};
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
                                   [this](const net::ConnectionId connectionId) {
                                     return onConnectionLost(connectionId);
                                   },
                                 .connectionReadyHandler =
                                   [this](net::ConnectionShPtr connection) {
                                     onConnectionReady(connection);
                                   }};

  m_tcpServer = std::make_shared<net::TcpServer>(m_context, port, config);
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

void Server::onConnectionLost(const net::ConnectionId connectionId)
{
  if (!m_clientManager->isStillConnected(connectionId))
  {
    m_clientManager->removeConnection(connectionId);
    return;
  }

  const auto data = m_clientManager->tryGetDataForConnection(connectionId);
  if (!data.playerDbId)
  {
    error("Connection " + std::to_string(connectionId)
          + " lost but could not find associated player");
  }

  if (data.stale)
  {
    debug("Connection " + std::to_string(connectionId) + " is already marked as stale");
    return;
  }

  info("Connection " + std::to_string(connectionId) + " lost but player " + str(*data.playerDbId)
       + " is still connected");

  m_clientManager->markConnectionAsStale(connectionId);
  auto message = std::make_unique<LogoutMessage>(*data.playerDbId, true);
  message->setClientId(data.clientId);

  m_messageExchanger->pushMessage(std::move(message));
}

void Server::onConnectionReady(net::ConnectionShPtr connection)
{
  const auto clientId = m_clientManager->registerConnection(connection);
  m_messageExchanger->registerConnection(clientId, connection);
}

} // namespace bsgo
