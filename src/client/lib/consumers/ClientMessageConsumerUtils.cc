
#include "ClientMessageConsumerUtils.hh"
#include "EntityService.hh"

#include "AsteroidListMessageConsumer.hh"
#include "ComponentMessageConsumer.hh"
#include "OutpostListMessageConsumer.hh"
#include "PlayerListMessageConsumer.hh"
#include "PlayerShipListMessageConsumer.hh"
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
  inputMessagesQueue.addListener(
    std::make_unique<PlayerListMessageConsumer>(entityMapper, coordinator));
  inputMessagesQueue.addListener(
    std::make_unique<AsteroidListMessageConsumer>(entityMapper, coordinator));
  inputMessagesQueue.addListener(
    std::make_unique<OutpostListMessageConsumer>(entityMapper, coordinator));
  inputMessagesQueue.addListener(
    std::make_unique<PlayerShipListMessageConsumer>(entityMapper, coordinator));
}

} // namespace pge
