
#include "MessageConsumerSetup.hh"

#include "DockMessageConsumer.hh"
#include "EntityAddedMessageConsumer.hh"
#include "EntityDeletedMessageConsumer.hh"
#include "EquipMessageConsumer.hh"
#include "HangarMessageConsumer.hh"
#include "JumpCancelledMessageConsumer.hh"
#include "JumpRequestedMessageConsumer.hh"
#include "LoadingMessagesConsumer.hh"
#include "PurchaseMessageConsumer.hh"
#include "SlotMessageConsumer.hh"
#include "TargetMessageConsumer.hh"
#include "VelocityMessageConsumer.hh"

namespace bsgo {

void createMessageConsumers(bsgalone::core::IMessageQueue *const inputMessagesQueue,
                            bsgalone::core::IMessageQueue *const outputMessagesQueue,
                            const Services &services)
{
  inputMessagesQueue->addListener(
    std::make_unique<HangarMessageConsumer>(services, inputMessagesQueue, outputMessagesQueue));

  inputMessagesQueue->addListener(
    std::make_unique<PurchaseMessageConsumer>(services, inputMessagesQueue, outputMessagesQueue));

  inputMessagesQueue->addListener(
    std::make_unique<EquipMessageConsumer>(services, inputMessagesQueue, outputMessagesQueue));

  inputMessagesQueue->addListener(
    std::make_unique<DockMessageConsumer>(services, inputMessagesQueue, outputMessagesQueue));

  inputMessagesQueue->addListener(
    std::make_unique<SlotMessageConsumer>(services, outputMessagesQueue));

  inputMessagesQueue->addListener(
    std::make_unique<VelocityMessageConsumer>(services, outputMessagesQueue));

  inputMessagesQueue->addListener(
    std::make_unique<TargetMessageConsumer>(services, outputMessagesQueue));

  inputMessagesQueue->addListener(
    std::make_unique<JumpCancelledMessageConsumer>(services, outputMessagesQueue));

  inputMessagesQueue->addListener(
    std::make_unique<JumpRequestedMessageConsumer>(services, outputMessagesQueue));

  inputMessagesQueue->addListener(
    std::make_unique<EntityDeletedMessageConsumer>(services, outputMessagesQueue));

  inputMessagesQueue->addListener(
    std::make_unique<EntityAddedMessageConsumer>(services, outputMessagesQueue));

  inputMessagesQueue->addListener(
    std::make_unique<LoadingMessagesConsumer>(services, outputMessagesQueue));
}

} // namespace bsgo
