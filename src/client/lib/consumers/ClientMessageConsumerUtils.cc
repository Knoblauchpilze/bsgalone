
#include "ClientMessageConsumerUtils.hh"

#include "SlotComponentMessageConsumer.hh"
#include "TargetMessageConsumer.hh"
#include "VelocityMessageConsumer.hh"

namespace pge {

void createMessageConsumers(bsgo::IMessageQueue &inputMessagesQueue,
                            bsgo::IMessageQueue *const outputMessagesQueue,
                            const bsgo::Services &services)
{
  inputMessagesQueue.addListener(
    std::make_unique<SlotComponentMessageConsumer>(services, outputMessagesQueue));

  auto velocity = std::make_unique<bsgo::VelocityMessageConsumer>(services, outputMessagesQueue);
  inputMessagesQueue.addListener(std::move(velocity));

  auto target = std::make_unique<bsgo::TargetMessageConsumer>(services, outputMessagesQueue);
  inputMessagesQueue.addListener(std::move(target));
}

} // namespace pge