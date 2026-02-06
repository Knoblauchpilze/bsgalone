
#include "MessageExchanger.hh"
#include "AiBehaviorSyncMessageConsumer.hh"
#include "AsyncMessageQueue.hh"
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

auto MessageExchanger::getInternalMessageQueue() const -> IMessageQueue *
{
  return m_internalMessageQueue.get();
}

namespace {
auto createInternalMessageQueue() -> IMessageQueueShPtr
{
  auto messageQueue = std::make_unique<SynchronizedMessageQueue>("synchronized-queue-for-internal");
  return std::make_shared<AsyncMessageQueue>(std::move(messageQueue));
}
} // namespace

void MessageExchanger::initialize(const MessageSystemData &messagesData)
{
  auto systemMessageQueue = initializeSystemMessageQueue(messagesData);
  m_internalMessageQueue  = createInternalMessageQueue();
  initializeInternalConsumers(messagesData);

  messagesData.networkClient->addListener(
    std::make_unique<TriageMessageConsumer>(messagesData.clientManager,
                                            messagesData.systemQueues,
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
  systemQueue->addListener(
    std::make_unique<SignupMessageConsumer>(std::move(signupService),
                                            messagesData.networkClient.get()));

  auto loginService = std::make_unique<LoginService>(repositories);
  systemQueue->addListener(std::make_unique<LoginMessageConsumer>(std::move(loginService),
                                                                  messagesData.clientManager,
                                                                  messagesData.systemQueues,
                                                                  messagesData.networkClient.get()));

  auto systemService = std::make_shared<SystemService>(repositories);
  systemQueue->addListener(
    std::make_unique<LogoutMessageConsumer>(systemService,
                                            messagesData.systemQueues,
                                            messagesData.networkClient.get()));

  auto playerService = std::make_unique<PlayerService>(repositories);
  systemQueue->addListener(
    std::make_unique<JoinShipMessageConsumer>(std::move(playerService),
                                              messagesData.networkClient.get()));

  return systemQueue;
}

void MessageExchanger::initializeInternalConsumers(const MessageSystemData &messagesData)
{
  Repositories repositories{};
  auto systemService = std::make_shared<SystemService>(std::move(repositories));

  m_internalMessageQueue->addListener(
    std::make_unique<LootMessageConsumer>(systemService, messagesData.networkClient.get()));

  m_internalMessageQueue->addListener(
    std::make_unique<JumpMessageConsumer>(systemService,
                                          messagesData.clientManager,
                                          messagesData.systemQueues,
                                          messagesData.networkClient.get()));

  m_internalMessageQueue->addListener(
    std::make_unique<EntityRemovedMessageConsumer>(systemService,
                                                   messagesData.systemQueues,
                                                   messagesData.networkClient.get()));

  m_internalMessageQueue->addListener(
    std::make_unique<RoutingMessageConsumer>(systemService, messagesData.networkClient.get()));

  m_internalMessageQueue->addListener(
    std::make_unique<AiBehaviorSyncMessageConsumer>(systemService,
                                                    messagesData.networkClient.get()));
}

} // namespace bsgo
