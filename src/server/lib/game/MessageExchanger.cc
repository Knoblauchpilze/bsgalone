
#include "MessageExchanger.hh"
#include "AiBehaviorSyncMessageConsumer.hh"
#include "AsyncMessageQueue.hh"
#include "BroadcastMessageQueue.hh"
#include "EntityRemovedMessageConsumer.hh"
#include "JoinShipMessageConsumer.hh"
#include "JumpMessageConsumer.hh"
#include "JumpService.hh"
#include "LoginMessageConsumer.hh"
#include "LogoutMessageConsumer.hh"
#include "LootMessageConsumer.hh"
#include "RoutingMessageConsumer.hh"
#include "SignupMessageConsumer.hh"
#include "SynchronizedMessageQueue.hh"
#include "TriageMessageConsumer.hh"

namespace bsgo {

MessageExchanger::MessageExchanger(const MessageSystemData &messagesData)
{
  initialize(messagesData);
}

auto MessageExchanger::getInputMessageQueue() const -> IMessageQueueShPtr
{
  return m_inputMessageQueue;
}

auto MessageExchanger::getInternalMessageQueue() const -> IMessageQueue *
{
  return m_internalMessageQueue.get();
}

auto MessageExchanger::getOutputMessageQueue() const -> IMessageQueue *
{
  return m_outputMessageQueue.get();
}

namespace {
auto createInputMessageQueue() -> IMessageQueueShPtr
{
  auto messageQueue = std::make_unique<SynchronizedMessageQueue>("synchronized-queue-for-input");
  return std::make_shared<AsyncMessageQueue>(std::move(messageQueue));
}

auto createInternalMessageQueue() -> IMessageQueuePtr
{
  auto messageQueue = std::make_unique<SynchronizedMessageQueue>("synchronized-queue-for-internal");
  return std::make_unique<AsyncMessageQueue>(std::move(messageQueue));
}

auto createOutputMessageQueue(ClientManagerShPtr clientManager, net::INetworkServerShPtr server)
  -> IMessageQueuePtr
{
  auto broadcastQueue = std::make_unique<BroadcastMessageQueue>(std::move(clientManager),
                                                                std::move(server));
  return std::make_unique<AsyncMessageQueue>(std::move(broadcastQueue));
}
} // namespace

void MessageExchanger::initialize(const MessageSystemData &messagesData)
{
  m_inputMessageQueue  = createInputMessageQueue();
  m_outputMessageQueue = createOutputMessageQueue(messagesData.clientManager, messagesData.server);
  auto systemMessageQueue = initializeSystemMessageQueue(messagesData);
  m_internalMessageQueue  = createInternalMessageQueue();
  initializeInternalConsumers(messagesData);

  m_inputMessageQueue->addListener(
    std::make_unique<TriageMessageConsumer>(messagesData.clientManager,
                                            messagesData.systemProcessors,
                                            std::move(systemMessageQueue)));
}

namespace {
auto createSystemMessageQueue() -> IMessageQueuePtr
{
  auto systemQueue = std::make_unique<SynchronizedMessageQueue>("synchronized-queue-for-system");
  return std::make_unique<AsyncMessageQueue>(std::move(systemQueue));
}
} // namespace

auto MessageExchanger::initializeSystemMessageQueue(const MessageSystemData &messagesData)
  -> IMessageQueuePtr
{
  Repositories repositories{};

  auto systemQueue = createSystemMessageQueue();

  auto signupService = std::make_unique<SignupService>(repositories);
  systemQueue->addListener(std::make_unique<SignupMessageConsumer>(std::move(signupService),
                                                                   messagesData.clientManager,
                                                                   messagesData.systemProcessors,
                                                                   m_outputMessageQueue.get()));

  auto loginService = std::make_unique<LoginService>(repositories);
  systemQueue->addListener(std::make_unique<LoginMessageConsumer>(std::move(loginService),
                                                                  messagesData.clientManager,
                                                                  messagesData.systemProcessors,
                                                                  m_outputMessageQueue.get()));

  auto systemService = std::make_shared<SystemService>(repositories);
  systemQueue->addListener(std::make_unique<LogoutMessageConsumer>(messagesData.clientManager,
                                                                   systemService,
                                                                   messagesData.systemProcessors,
                                                                   m_outputMessageQueue.get()));

  auto playerService = std::make_unique<PlayerService>(repositories);
  systemQueue->addListener(std::make_unique<JoinShipMessageConsumer>(std::move(playerService),
                                                                     m_outputMessageQueue.get()));

  return systemQueue;
}

void MessageExchanger::initializeInternalConsumers(const MessageSystemData &messagesData)
{
  Repositories repositories{};
  auto systemService = std::make_shared<SystemService>(std::move(repositories));

  m_internalMessageQueue->addListener(
    std::make_unique<LootMessageConsumer>(systemService, m_outputMessageQueue.get()));

  m_internalMessageQueue->addListener(
    std::make_unique<JumpMessageConsumer>(systemService,
                                          messagesData.clientManager,
                                          messagesData.systemProcessors,
                                          m_outputMessageQueue.get()));

  m_internalMessageQueue->addListener(
    std::make_unique<EntityRemovedMessageConsumer>(systemService,
                                                   messagesData.systemProcessors,
                                                   m_outputMessageQueue.get()));

  m_internalMessageQueue->addListener(
    std::make_unique<RoutingMessageConsumer>(systemService, m_outputMessageQueue.get()));

  m_internalMessageQueue->addListener(
    std::make_unique<AiBehaviorSyncMessageConsumer>(systemService, m_outputMessageQueue.get()));
}

} // namespace bsgo
