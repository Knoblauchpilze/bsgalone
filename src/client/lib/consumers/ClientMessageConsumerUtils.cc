
#include "ClientMessageConsumerUtils.hh"

#include "ComponentMessageConsumer.hh"
#include "ShipMessageConsumer.hh"
#include "SystemMessageConsumer.hh"

namespace pge {

void createMessageConsumers(bsgo::IMessageQueue &inputMessagesQueue,
                            bsgo::DatabaseEntityMapper &entityMapper,
                            const bsgo::CoordinatorShPtr &coordinator)
{
  inputMessagesQueue.addListener(
    std::make_unique<ComponentMessageConsumer>(entityMapper, coordinator));

  inputMessagesQueue.addListener(std::make_unique<ShipMessageConsumer>(entityMapper, coordinator));

  inputMessagesQueue.addListener(std::make_unique<SystemMessageConsumer>(entityMapper, coordinator));
}

} // namespace pge
