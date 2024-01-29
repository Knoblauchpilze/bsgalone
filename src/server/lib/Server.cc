

#include "Server.hh"
#include "AsyncMessageQueue.hh"
#include "ConnectionMessage.hh"
#include "DataSource.hh"
#include "NetworkMessageQueue.hh"
#include "SynchronizedMessageQueue.hh"
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

namespace {
auto createInputMessageQueue() -> NetworkMessageQueuePtr
{
  auto messageQueue = std::make_unique<SynchronizedMessageQueue>();
  auto asyncQueue   = std::make_unique<AsyncMessageQueue>(std::move(messageQueue));

  return std::make_unique<NetworkMessageQueue>(std::move(asyncQueue));
}

auto createOutputMessageQueue(IMessageQueuePtr queue) -> IMessageQueuePtr
{
  return std::make_unique<AsyncMessageQueue>(std::move(queue));
}
} // namespace

void Server::initialize()
{
  m_inputMessageQueue  = createInputMessageQueue();
  auto broadcastQueue  = std::make_unique<BroadcastMessageQueue>();
  m_broadcastQueue     = broadcastQueue.get();
  m_outputMessageQueue = createOutputMessageQueue(std::move(broadcastQueue));

  initializeSystems();
}

void Server::initializeSystems()
{
  DataSource source{DataLoadingMode::SERVER};
  const auto allSystems = source.repositories().systemRepository->findAll();

  for (const auto &systemDbId : allSystems)
  {
    SystemProcessingConfig config{.systemDbId          = systemDbId,
                                  .inputMessageQueue   = m_inputMessageQueue.get(),
                                  .outputMessageQueue  = m_outputMessageQueue.get(),
                                  .playerLoginCallback = [this](const Uuid clientId,
                                                                const Uuid playerDbId) {
                                    m_broadcastQueue->registerPlayer(clientId, playerDbId);
                                  }};

    auto processor = std::make_unique<SystemProcessor>(config);
    m_systemProcessors.emplace_back(std::move(processor));
  }
}

void Server::setup(const int port)
{
  const net::ServerConfig config{.acceptor =
                                   [this](const net::Connection &connection) {
                                     return onConnectionReceived(connection);
                                   },
                                 .disconnectHandler =
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

bool Server::onConnectionReceived(const net::Connection & /*connection*/) const
{
  return true;
}

void Server::onConnectionLost(const net::ConnectionId connectionId)
{
  m_clientManager.removeConnection(connectionId);
}

void Server::onConnectionReady(net::ConnectionShPtr connection)
{
  const auto clientId = m_clientManager.registerConnection(connection->id());

  m_inputMessageQueue->registerToConnection(*connection);
  m_broadcastQueue->registerClient(clientId, connection);

  auto message = std::make_unique<ConnectionMessage>(clientId);
  message->validate();
  m_outputMessageQueue->pushMessage(std::move(message));
}

} // namespace bsgo
