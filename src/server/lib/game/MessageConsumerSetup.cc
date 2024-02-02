
#include "MessageConsumerSetup.hh"

#include "DockMessageConsumer.hh"
#include "EquipMessageConsumer.hh"
#include "HangarMessageConsumer.hh"
#include "JumpCancelledMessageConsumer.hh"
#include "JumpMessageConsumer.hh"
#include "JumpRequestedMessageConsumer.hh"
#include "PurchaseMessageConsumer.hh"
#include "ScannedMessageConsumer.hh"
#include "SlotMessageConsumer.hh"

namespace bsgo {

void createMessageConsumers(IMessageQueue &inputMessagesQueue,
                            IMessageQueue *const outputMessagesQueue,
                            const Services &services)
{
  inputMessagesQueue.addListener(
    std::make_unique<HangarMessageConsumer>(services, outputMessagesQueue));

  inputMessagesQueue.addListener(
    std::make_unique<PurchaseMessageConsumer>(services, outputMessagesQueue));

  inputMessagesQueue.addListener(
    std::make_unique<EquipMessageConsumer>(services, outputMessagesQueue));

  inputMessagesQueue.addListener(
    std::make_unique<DockMessageConsumer>(services, outputMessagesQueue));

  inputMessagesQueue.addListener(std::make_unique<SlotMessageConsumer>(services));

  inputMessagesQueue.addListener(std::make_unique<ScannedMessageConsumer>(outputMessagesQueue));

  inputMessagesQueue.addListener(
    std::make_unique<JumpMessageConsumer>(services, outputMessagesQueue));

  inputMessagesQueue.addListener(
    std::make_unique<JumpCancelledMessageConsumer>(services, outputMessagesQueue));

  inputMessagesQueue.addListener(
    std::make_unique<JumpRequestedMessageConsumer>(services, outputMessagesQueue));
}

} // namespace bsgo
