
#include "ClientMessageConsumerUtils.hh"

#include "AbstractGameMessageConsumer.hh"
#include "AsteroidListMessageConsumer.hh"
#include "ComponentMessageConsumer.hh"
#include "GameMessageConsumerProxy.hh"
#include "OutpostListMessageConsumer.hh"
#include "PlayerListMessageConsumer.hh"
#include "PlayerShipListMessageConsumer.hh"
#include "ShipMessageConsumer.hh"
#include "SystemMessageConsumer.hh"
#include "TargetListMessageConsumer.hh"

namespace pge {

auto wrapAndRegisterToQueue(bsgo::AbstractMessageConsumerPtr consumer,
                            bsgo::IMessageQueue &inputMessagesQueue)
  -> bsgo::AbstractMessageConsumerPtr
{
  // All game consumers are deactivated at the start because the players
  // start in the outpost.
  auto gameConsumer = std::make_unique<AbstractGameMessageConsumer>(false, std::move(consumer));
  inputMessagesQueue.addListener(std::make_unique<GameMessageConsumerProxy>(*gameConsumer));
  return gameConsumer;
}

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
  inputMessagesQueue.addListener(
    std::make_unique<TargetListMessageConsumer>(entityMapper, coordinator));
}

} // namespace pge
