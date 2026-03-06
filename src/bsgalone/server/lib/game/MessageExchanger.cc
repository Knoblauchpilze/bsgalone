
#include "MessageExchanger.hh"
#include "AiBehaviorSyncMessageConsumer.hh"
#include "AsyncMessageQueue.hh"
#include "EntityRemovedMessageConsumer.hh"
#include "JumpMessageConsumer.hh"
#include "LootMessageConsumer.hh"
#include "RoutingMessageConsumer.hh"
#include "SynchronizedMessageQueue.hh"

namespace bsgalone::server {

MessageExchanger::MessageExchanger(const MessageSystemData &messagesData)
{
  initialize(messagesData);
}

auto MessageExchanger::getInternalMessageQueue() const -> core::IMessageQueue *
{
  return m_internalMessageQueue.get();
}

namespace {
auto createInternalMessageQueue() -> core::IMessageQueueShPtr
{
  auto messageQueue = core::createSynchronizedMessageQueue();
  return core::createAsyncMessageQueue(std::move(messageQueue));
}
} // namespace

void MessageExchanger::initialize(const MessageSystemData &messagesData)
{
  m_internalMessageQueue = createInternalMessageQueue();
  initializeInternalConsumers(messagesData);
}

void MessageExchanger::initializeInternalConsumers(const MessageSystemData &messagesData)
{
  core::Repositories repositories{};
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

} // namespace bsgalone::server
