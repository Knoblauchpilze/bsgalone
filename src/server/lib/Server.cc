

#include "Server.hh"
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
  const MessageSystemData data{.networkClient = m_networkClient, .systemQueues = m_inputQueues};
  m_messageExchanger = std::make_unique<MessageExchanger>(data);

  for (const auto &systemProcessor : m_systemProcessors)
  {
    systemProcessor->connectToQueues(m_messageExchanger->getInternalMessageQueue(),
                                     m_networkClient.get());
  }
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
  m_networkClient->stop();
}

} // namespace bsgo
