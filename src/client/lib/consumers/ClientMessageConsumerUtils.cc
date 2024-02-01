
#include "ClientMessageConsumerUtils.hh"

#include "JumpMessageConsumer.hh"
#include "SlotComponentMessageConsumer.hh"
#include "TargetMessageConsumer.hh"
#include "VelocityMessageConsumer.hh"

namespace pge {

void createMessageConsumers(bsgo::IMessageQueue &inputMessagesQueue,
                            bsgo::IMessageQueue *const outputMessagesQueue,
                            const bsgo::Services &services,
                            const bsgo::DatabaseEntityMapper &entityMapper,
                            const bsgo::CoordinatorShPtr &coordinator)
{
  inputMessagesQueue.addListener(
    std::make_unique<SlotComponentMessageConsumer>(entityMapper, coordinator));

  inputMessagesQueue.addListener(std::make_unique<JumpMessageConsumer>(entityMapper, coordinator));

  auto velocity = std::make_unique<bsgo::VelocityMessageConsumer>(services, outputMessagesQueue);
  inputMessagesQueue.addListener(std::move(velocity));

  auto target = std::make_unique<bsgo::TargetMessageConsumer>(services, outputMessagesQueue);
  inputMessagesQueue.addListener(std::move(target));
}

} // namespace pge
