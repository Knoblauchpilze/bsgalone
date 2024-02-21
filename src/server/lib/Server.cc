

#include "Server.hh"
#include "DataSource.hh"
#include "LogoutMessage.hh"
#include "SystemProcessorUtils.hh"
#include <core_utils/TimeUtils.hh>

namespace bsgo {

Server::Server()
  : utils::CoreObject("server")
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
  DataSource source{DataLoadingMode::SERVER};
  const auto repositories = source.repositories();

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

  m_systemProcessors.clear();
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

  const auto maybeData = m_clientManager->tryGetDataForConnection(connectionId);
  if (!maybeData.playerDbId)
  {
    error("Connection " + std::to_string(connectionId)
          + " lost but could not find associated player");
  }

  info("Connection " + std::to_string(connectionId) + " lost but player "
       + str(*maybeData.playerDbId) + " is still connected");

  auto message = std::make_unique<LogoutMessage>(*maybeData.playerDbId, true);
  message->setClientId(maybeData.clientId);

  m_messageExchanger->pushMessage(std::move(message));
}

void Server::onConnectionReady(net::ConnectionShPtr connection)
{
  const auto clientId = m_clientManager->registerConnection(connection);
  m_messageExchanger->registerConnection(clientId, connection);
}

} // namespace bsgo
