
#include "MessageExchanger.hh"
#include "AsyncMessageQueue.hh"
#include "BroadcastMessageQueue.hh"
#include "ConnectionMessage.hh"
#include "DataSource.hh"
#include "EntityRemovedMessageConsumer.hh"
#include "LoginMessageConsumer.hh"
#include "LogoutMessageConsumer.hh"
#include "LootMessageConsumer.hh"
#include "SignupMessageConsumer.hh"
#include "SynchronizedMessageQueue.hh"
#include "TriageMessageConsumer.hh"

namespace bsgo {

MessageExchanger::MessageExchanger(const ClientManagerShPtr &clientManager,
                                   const std::vector<SystemProcessorShPtr> &systemProcessors)
{
  initialize(clientManager, systemProcessors);
}

auto MessageExchanger::getInternalMessageQueue() const -> IMessageQueue *
{
  return m_internalMessageQueue.get();
}

auto MessageExchanger::getOutputMessageQueue() const -> IMessageQueue *
{
  return m_outputMessageQueue.get();
}

void MessageExchanger::registerConnection(const Uuid clientId, net::ConnectionShPtr connection)
{
  m_inputMessageQueue->registerToConnection(*connection);

  auto message = std::make_unique<ConnectionMessage>(clientId);
  message->validate();
  m_outputMessageQueue->pushMessage(std::move(message));
}

void MessageExchanger::initialize(const ClientManagerShPtr &clientManager,
                                  const std::vector<SystemProcessorShPtr> &systemProcessors)
{
  initializeQueues(clientManager);
  initializeConsumers(clientManager, systemProcessors);
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

void MessageExchanger::initializeQueues(ClientManagerShPtr clientManager)
{
  m_inputMessageQueue  = createInputMessageQueue();
  m_outputMessageQueue = createOutputMessageQueue(std::move(clientManager));
}

namespace {
auto createSystemMessageQueue() -> IMessageQueuePtr
{
  auto systemQueue = std::make_unique<SynchronizedMessageQueue>();
  return std::make_unique<AsyncMessageQueue>(std::move(systemQueue));
}

auto createInternalMessageQueue() -> IMessageQueuePtr
{
  auto messageQueue = std::make_unique<SynchronizedMessageQueue>();
  return std::make_unique<AsyncMessageQueue>(std::move(messageQueue));
}

} // namespace

void MessageExchanger::initializeConsumers(const ClientManagerShPtr &clientManager,
                                           const std::vector<SystemProcessorShPtr> &systemProcessors)
{
  DataSource source{DataLoadingMode::SERVER};
  const auto repositories = source.repositories();

  auto systemQueue = createSystemMessageQueue();

  auto signupService = std::make_unique<SignupService>(repositories);
  systemQueue->addListener(std::make_unique<SignupMessageConsumer>(std::move(signupService),
                                                                   clientManager,
                                                                   m_outputMessageQueue.get()));

  auto loginService = std::make_unique<LoginService>(repositories);
  systemQueue->addListener(std::make_unique<LoginMessageConsumer>(std::move(loginService),
                                                                  clientManager,
                                                                  m_outputMessageQueue.get()));

  systemQueue->addListener(
    std::make_unique<LogoutMessageConsumer>(clientManager, m_outputMessageQueue.get()));

  initializeInternalMessageQueue(repositories, systemProcessors);

  auto triageConsumer = std::make_unique<TriageMessageConsumer>(systemProcessors,
                                                                clientManager,
                                                                std::move(systemQueue));
  m_inputMessageQueue->addListener(std::move(triageConsumer));
}

void MessageExchanger::initializeInternalMessageQueue(
  const Repositories &repositories,
  const std::vector<SystemProcessorShPtr> &systemProcessors)
{
  m_internalMessageQueue = createInternalMessageQueue();

  auto combatService = std::make_shared<CombatService>(repositories);
  m_internalMessageQueue->addListener(
    std::make_unique<LootMessageConsumer>(combatService, m_outputMessageQueue.get()));

  m_internalMessageQueue->addListener(
    std::make_unique<EntityRemovedMessageConsumer>(combatService,
                                                   systemProcessors,
                                                   m_outputMessageQueue.get()));
}

} // namespace bsgo
