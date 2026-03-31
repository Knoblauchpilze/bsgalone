

#include "Server.hh"
#include "AsyncGameEventQueue.hh"
#include "Configurator.hh"
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
  m_networkClient = std::make_shared<ServerNetworkClient>();

  initializeExternalFacingUseCases();

  auto outputAdapter
    = std::make_unique<core::OutputNetworkAdapter>(m_networkClient,
                                                   std::make_unique<core::MessageSerializer>());
  m_eventQueue->addListener(std::make_unique<OutputGameEventAdapter>(std::move(outputAdapter)));
}

void Server::initializeExternalFacingUseCases()
{
  m_eventQueue   = createAsyncGameEventQueue(createSynchronizedGameEventQueue());
  auto publisher = std::make_shared<core::GameEventPublisher>(m_eventQueue);

  Configurator configurator{};

  m_networkClient->addListener(configurator.createLoginDrivingAdapter(publisher));
}

void Server::setup(const int port)
{
  info("Starting listening on port " + std::to_string(port));
  m_networkClient->start(port);
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
  m_networkClient->stop();
  m_eventQueue.reset();
}

} // namespace bsgalone::server
