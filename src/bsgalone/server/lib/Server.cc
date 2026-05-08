

#include "Server.hh"
#include "AsyncGameEventQueue.hh"
#include "ClientManager.hh"
#include "Configurator.hh"
#include "EcsCoordinator.hh"
#include "GameEventPublisher.hh"
#include "MessageSerializer.hh"
#include "OutputGameEventAdapter.hh"
#include "OutputNetworkAdapter.hh"
#include "SynchronizedGameEventQueue.hh"

namespace bsgalone::server {

Server::Server()
  : ::core::CoreObject("server")
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
  auto clientManager = std::make_shared<ClientManager>();
  m_networkClient    = std::make_shared<ServerNetworkClient>(clientManager);
  m_clientManager    = std::move(clientManager);

  initializeSystemProcessors();
  initializeInboundUseCases();
  initializeOutboundUseCases();
}

void Server::initializeSystemProcessors()
{
  auto connection = std::make_shared<DbConnection>();
  connection->connect();
  SystemRepository repository(std::move(connection));
  repository.initialize();

  for (const auto &system : repository.findAll())
  {
    auto manager = std::make_unique<chrono::TimeManager>(system.currentTick, system.step);
    // TODO: The registry should be kept locally to initialize other things
    auto coordinator = std::make_unique<core::EcsCoordinator>(
      std::make_shared<core::EntityRegistry>());
    auto processor = std::make_shared<core::SystemProcessor>(system.name,
                                                             std::move(coordinator),
                                                             std::move(manager));
    m_systemProcessors.emplace_back(std::move(processor));
  }
}

void Server::initializeInboundUseCases()
{
  m_eventQueue   = createAsyncGameEventQueue(createSynchronizedGameEventQueue());
  auto publisher = std::make_shared<GameEventPublisher>(m_eventQueue);

  Configurator configurator{};

  m_networkClient->addListener(configurator.createSignupDrivingAdapter(publisher));
  m_networkClient->addListener(configurator.createLoginDrivingAdapter(m_clientManager, publisher));
  m_networkClient->addListener(configurator.createLogoutDrivingAdapter(m_clientManager, publisher));
  m_networkClient->addListener(configurator.createUndockDrivingAdapter(publisher));
}

void Server::initializeOutboundUseCases()
{
  auto outputAdapter
    = std::make_unique<core::OutputNetworkAdapter>(m_networkClient,
                                                   std::make_unique<core::MessageSerializer>());
  m_eventQueue->addListener(
    std::make_unique<OutputGameEventAdapter>(m_clientManager, std::move(outputAdapter)));
}

void Server::setup(const int port)
{
  info("Starting listening on port " + std::to_string(port));
  m_networkClient->start(port);

  for (const auto &processor : m_systemProcessors)
  {
    processor->start();
  }
}

void Server::activeRunLoop()
{
  m_running.store(true);
  bool running{true};

  while (running)
  {
    std::unique_lock lock(m_runningLocker);
    m_runningNotifier.wait(lock, [this] { return !m_running.load(); });

    running = m_running.load();
  }
}

void Server::shutdown()
{
  for (const auto &systemProcessor : m_systemProcessors)
  {
    systemProcessor->stop();
  }

  m_networkClient->stop();
  m_eventQueue.reset();
}

} // namespace bsgalone::server
