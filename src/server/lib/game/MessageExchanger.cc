
#include "MessageExchanger.hh"
#include "AiBehaviorSyncMessageConsumer.hh"
#include "AsyncMessageQueue.hh"
#include "EntityRemovedMessageConsumer.hh"
#include "JumpMessageConsumer.hh"
#include "LootMessageConsumer.hh"
#include "RoutingMessageConsumer.hh"
#include "SynchronizedMessageQueue.hh"

namespace bsgo {

MessageExchanger::MessageExchanger(const MessageSystemData &messagesData)
{
  initialize(messagesData);
}

auto MessageExchanger::getInternalMessageQueue() const -> bsgalone::core::IMessageQueue *
{
  return m_internalMessageQueue.get();
}

namespace {
auto createInternalMessageQueue() -> bsgalone::core::IMessageQueueShPtr
{
  auto messageQueue = std::make_unique<SynchronizedMessageQueue>("synchronized-queue-for-internal");
  return std::make_shared<AsyncMessageQueue>(std::move(messageQueue));
}
} // namespace

void MessageExchanger::initialize(const MessageSystemData &messagesData)
{
  m_internalMessageQueue = createInternalMessageQueue();
  initializeInternalConsumers(messagesData);
}

void MessageExchanger::initializeInternalConsumers(const MessageSystemData &messagesData)
{
  Repositories repositories{};
  auto systemService = std::make_shared<SystemService>(std::move(repositories));

  m_internalMessageQueue->addListener(
    std::make_unique<LootMessageConsumer>(systemService, messagesData.networkClient.get()));

  m_internalMessageQueue->addListener(
    std::make_unique<JumpMessageConsumer>(systemService,
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
