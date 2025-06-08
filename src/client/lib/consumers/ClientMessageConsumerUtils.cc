
#include "ClientMessageConsumerUtils.hh"
#include "EntityService.hh"

#include "ComponentMessageConsumer.hh"
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

  // TODO: We could create a new LoadingMessageConsumer which would take the coordinator
  // and the entity mapper and would add the players, ships, etc. in a similar way to
  // how it's done in the PlayerDataSource::initialize method.
  // It would be responsible to call the PlayerDataSource::registerPlayer with the data
  // that is coming from the PlayerListMessage for example instead of fetching it from
  // the database.
}

} // namespace pge
