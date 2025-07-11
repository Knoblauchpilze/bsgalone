
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

void createMessageConsumers(IMessageQueue *const inputMessagesQueue,
                            IMessageQueue *const outputMessagesQueue,
                            const Services &services)
{
  inputMessagesQueue->addListener(
    std::make_unique<HangarMessageConsumer>(services, outputMessagesQueue));

  inputMessagesQueue->addListener(
    std::make_unique<PurchaseMessageConsumer>(services, outputMessagesQueue));

  inputMessagesQueue->addListener(
    std::make_unique<EquipMessageConsumer>(services, outputMessagesQueue));

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
