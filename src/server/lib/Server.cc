

#include "Server.hh"
#include "DataSource.hh"
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
  m_messageExchanger = std::make_unique<MessageExchanger>(m_clientManager, m_systemProcessors);
  for (const auto &systemProcessor : m_systemProcessors)
  {
    systemProcessor->connectToQueue(m_messageExchanger->getOutputMessageQueue());
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

  m_tcpServer = std::make_unique<net::TcpServer>(m_context, port, config);

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
    constexpr auto SLEEP_DURATION_WHEN_POLLING_STATUS = utils::Milliseconds(100);
    std::this_thread::sleep_for(SLEEP_DURATION_WHEN_POLLING_STATUS);

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
  m_clientManager->removeConnection(connectionId);
}

void Server::onConnectionReady(net::ConnectionShPtr connection)
{
  const auto clientId = m_clientManager->registerConnection(connection);
  m_messageExchanger->registerConnection(clientId, connection);
}

} // namespace bsgo
