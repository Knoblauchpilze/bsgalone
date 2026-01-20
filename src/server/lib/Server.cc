

#include "Server.hh"
#include "AsyncEventBus.hh"
#include "LogoutMessage.hh"
#include "SynchronizedEventBus.hh"
#include "SystemProcessorUtils.hh"
#include "TcpServer.hh"

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
  m_messageExchanger->registerToEventBus(*m_eventBus);
  m_clientManager->registerToEventBus(*m_eventBus);

  for (const auto &systemProcessor : m_systemProcessors)
  {
    systemProcessor->connectToQueues(m_messageExchanger->getInternalMessageQueue(),
                                     m_messageExchanger->getOutputMessageQueue());
  }
}

void Server::setup(const int port)
{
  m_eventBus  = std::make_shared<net::AsyncEventBus>(std::make_unique<net::SynchronizedEventBus>());
  m_tcpServer = std::make_unique<net::TcpServer>(m_eventBus);

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

// TODO: This should be connected to the event bus
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

} // namespace bsgo
