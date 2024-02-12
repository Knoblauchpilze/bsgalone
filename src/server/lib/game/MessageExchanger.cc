
#include "MessageExchanger.hh"
#include "AsyncMessageQueue.hh"
#include "BroadcastMessageQueue.hh"
#include "ConnectionMessage.hh"
#include "DataSource.hh"
#include "EntityRemovedMessageConsumer.hh"
#include "JumpMessageConsumer.hh"
#include "JumpService.hh"
#include "LoginMessageConsumer.hh"
#include "LogoutMessageConsumer.hh"
#include "LootMessageConsumer.hh"
#include "SignupMessageConsumer.hh"
#include "SynchronizedMessageQueue.hh"
#include "TriageMessageConsumer.hh"

namespace bsgo {

MessageExchanger::MessageExchanger(const ClientManagerShPtr &clientManager,
                                   const SystemProcessorMap &systemProcessors)
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

void MessageExchanger::pushMessage(IMessagePtr message)
{
  m_inputMessageQueue->pushMessage(std::move(message));
}

void MessageExchanger::initialize(const ClientManagerShPtr &clientManager,
                                  const SystemProcessorMap &systemProcessors)
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
                                           const SystemProcessorMap &systemProcessors)
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

  auto systemService = std::make_shared<SystemService>(repositories);
  systemQueue->addListener(std::make_unique<LogoutMessageConsumer>(clientManager,
                                                                   systemService,
                                                                   systemProcessors,
                                                                   m_outputMessageQueue.get()));

  m_inputMessageQueue->addListener(
    std::make_unique<JumpMessageConsumer>(systemService,
                                          clientManager,
                                          systemProcessors,
                                          m_outputMessageQueue.get()));

  initializeInternalMessageQueue(systemService, systemProcessors);

  m_inputMessageQueue->addListener(std::make_unique<TriageMessageConsumer>(clientManager,
                                                                           systemProcessors,
                                                                           std::move(systemQueue)));
}

void MessageExchanger::initializeInternalMessageQueue(const SystemServiceShPtr &systemService,
                                                      const SystemProcessorMap &systemProcessors)
{
  m_internalMessageQueue = createInternalMessageQueue();

  m_internalMessageQueue->addListener(
    std::make_unique<LootMessageConsumer>(systemService, m_outputMessageQueue.get()));

  m_internalMessageQueue->addListener(
    std::make_unique<EntityRemovedMessageConsumer>(systemService,
                                                   systemProcessors,
                                                   m_outputMessageQueue.get()));
}

} // namespace bsgo
