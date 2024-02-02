
#include "ClientMessageConsumerUtils.hh"

#include "ShipMessageConsumer.hh"
#include "SlotComponentMessageConsumer.hh"
#include "SystemMessageConsumer.hh"
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

  inputMessagesQueue.addListener(std::make_unique<ShipMessageConsumer>(entityMapper, coordinator));

  inputMessagesQueue.addListener(std::make_unique<SystemMessageConsumer>(entityMapper, coordinator));

  auto velocity = std::make_unique<bsgo::VelocityMessageConsumer>(services, outputMessagesQueue);
  inputMessagesQueue.addListener(std::move(velocity));
}

} // namespace pge
