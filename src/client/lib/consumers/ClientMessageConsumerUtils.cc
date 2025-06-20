
#include "ClientMessageConsumerUtils.hh"
#include "EntityService.hh"

#include "ComponentMessageConsumer.hh"
#include "PlayerListMessageConsumer.hh"
#include "ShipMessageConsumer.hh"
#include "SystemMessageConsumer.hh"

namespace pge {

void createMessageConsumers(const bsgo::Repositories &repositories,
                            bsgo::IMessageQueue &inputMessagesQueue,
                            bsgo::DatabaseEntityMapper &entityMapper,
                            const bsgo::CoordinatorShPtr &coordinator)
{
  inputMessagesQueue.addListener(
    std::make_unique<ComponentMessageConsumer>(entityMapper, coordinator));

  inputMessagesQueue.addListener(std::make_unique<ShipMessageConsumer>(entityMapper, coordinator));

  // TODO: We should try to make the repositories a dummy value in the client
  // and see if anything breaks.
  auto entityService = std::make_unique<bsgo::EntityService>(bsgo::ProcessingMode::CLIENT,
                                                             repositories,
                                                             coordinator,
                                                             entityMapper);
  inputMessagesQueue.addListener(
    std::make_unique<SystemMessageConsumer>(entityMapper, coordinator, std::move(entityService)));

  inputMessagesQueue.addListener(
    std::make_unique<PlayerListMessageConsumer>(entityMapper, coordinator));
}

} // namespace pge
