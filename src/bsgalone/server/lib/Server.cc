

#include "Server.hh"
#include "JoinShipMessageConsumer.hh"
#include "LoginRequestConsumer.hh"
#include "LoginService.hh"
#include "LogoutMessageConsumer.hh"
#include "PlayerService.hh"
#include "SignupMessageConsumer.hh"
#include "SignupService.hh"
#include "SystemProcessorAdapter.hh"

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
  initializeSystems();
  initializeMessageSystem();
}

void Server::initializeSystems()
{
  const core::Repositories repositories;

  const auto allSystems = repositories.systemRepository->findAll();

  for (const auto &system : allSystems)
  {
    core::IMessageQueueShPtr inputQueue = core::createSynchronizedMessageQueue();
    auto processor             = std::make_shared<SystemProcessor>(system.dbId, inputQueue);
    m_inputQueues[system.dbId] = std::move(inputQueue);
    m_systemProcessors.emplace_back(std::move(processor));
  }
}

namespace {
void createSystemMessageConsumers(core::IMessageQueue &inputMessagesQueue,
                                  SystemQueueMap systemQueues,
                                  core::IMessageQueue *const outputMessagesQueue)
{
  core::Repositories repositories{};

  auto signupService = std::make_unique<SignupService>(repositories);
  inputMessagesQueue.addListener(
    std::make_unique<SignupMessageConsumer>(std::move(signupService), outputMessagesQueue));

  auto loginService = std::make_unique<LoginService>(repositories);
  inputMessagesQueue.addListener(std::make_unique<LoginRequestConsumer>(std::move(loginService),
                                                                        systemQueues,
                                                                        outputMessagesQueue));

  auto systemService = std::make_shared<SystemService>(repositories);
  inputMessagesQueue.addListener(
    std::make_unique<LogoutMessageConsumer>(systemService, systemQueues, outputMessagesQueue));

  auto playerService = std::make_unique<PlayerService>(repositories);
  inputMessagesQueue.addListener(
    std::make_unique<JoinShipMessageConsumer>(std::move(playerService), outputMessagesQueue));
}
} // namespace

void Server::initializeMessageSystem()
{
  const MessageSystemData data{.networkClient = m_networkClient, .systemQueues = m_inputQueues};
  m_messageExchanger = std::make_unique<MessageExchanger>(data);

  createSystemMessageConsumers(*m_networkClient, m_inputQueues, m_networkClient.get());

  for (const auto &systemProcessor : m_systemProcessors)
  {
    systemProcessor->connectToQueues(m_messageExchanger->getInternalMessageQueue(), m_networkClient);
  }

  for (const auto &[systemDbId, systemQueue] : m_inputQueues)
  {
    auto adapter = std::make_unique<SystemProcessorAdapter>(systemDbId, systemQueue);
    m_networkClient->addListener(std::move(adapter));
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

} // namespace bsgalone::server
