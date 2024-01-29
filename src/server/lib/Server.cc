

#include "Server.hh"
#include "AsyncMessageQueue.hh"
#include "ConnectionMessage.hh"
#include "LoginMessageConsumer.hh"
#include "SignupMessageConsumer.hh"
#include "SynchronizedMessageQueue.hh"
#include "TriageMessageConsumer.hh"
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

auto createOutputMessageQueue(ClientManagerShPtr clientManager) -> IMessageQueuePtr
{
  auto broadcastQueue = std::make_unique<BroadcastMessageQueue>(std::move(clientManager));
  return std::make_unique<AsyncMessageQueue>(std::move(broadcastQueue));
}
} // namespace

void Server::initialize()
{
  m_inputMessageQueue  = createInputMessageQueue();
  m_outputMessageQueue = createOutputMessageQueue(m_clientManager);

  initializeSystems();
  initializeMessageSystem();
}

void Server::initializeSystems()
{
  /// TODO: Should not simulate a single system.
  SystemProcessingConfig config{.systemDbId         = Uuid{0},
                                .outputMessageQueue = m_outputMessageQueue.get()};

  auto processor = std::make_shared<SystemProcessor>(config);
  m_systemProcessors.emplace_back(std::move(processor));
}

void Server::initializeMessageSystem()
{
  auto dbConnection = std::make_shared<DbConnection>();
  dbConnection->connect();
  const auto repositories = createRepositories(dbConnection);

  auto systemQueue = std::make_unique<SynchronizedMessageQueue>();

  auto signupService = std::make_unique<SignupService>(repositories);
  systemQueue->addListener(std::make_unique<SignupMessageConsumer>(std::move(signupService),
                                                                   m_clientManager,
                                                                   m_outputMessageQueue.get()));

  auto loginService = std::make_unique<LoginService>(repositories);
  systemQueue->addListener(std::make_unique<LoginMessageConsumer>(std::move(loginService),
                                                                  m_clientManager,
                                                                  m_outputMessageQueue.get()));

  auto triageConsumer = std::make_unique<TriageMessageConsumer>(m_systemProcessors,
                                                                m_clientManager,
                                                                std::move(systemQueue));
  m_inputMessageQueue->addListener(std::move(triageConsumer));
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

  m_inputMessageQueue->registerToConnection(*connection);

  auto message = std::make_unique<ConnectionMessage>(clientId);
  message->validate();
  m_outputMessageQueue->pushMessage(std::move(message));
}

} // namespace bsgo
