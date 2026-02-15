
#include "Views.hh"
#include "ViewConsumerProxy.hh"

namespace pge {

auto createViews(const ViewsConfig &config, const bsgo::DatabaseEntityMapper &entityMapper) -> Views
{
  Views out{};

  out.shipView     = std::make_shared<ShipView>(config.gameSession,
                                            config.coordinator,
                                            config.outputMessageQueue);
  out.shipDbView   = std::make_shared<ShipDbView>(config.gameSession,
                                                config.internalMessageQueue,
                                                config.outputMessageQueue);
  out.systemView   = std::make_shared<SystemView>(config.coordinator, entityMapper);
  out.playerView   = std::make_shared<PlayerView>(config.gameSession, config.outputMessageQueue);
  out.shopView     = std::make_shared<ShopView>(config.gameSession);
  out.serverView   = std::make_shared<ServerView>(config.gameSession);
  out.resourceView = std::make_shared<ResourceView>();

  return out;
}

void Views::connectToQueue(bsgalone::core::IMessageQueue *const queue)
{
  queue->addListener(std::make_unique<ViewConsumerProxy>(*playerView));
  queue->addListener(std::make_unique<ViewConsumerProxy>(*shipView));
  queue->addListener(std::make_unique<ViewConsumerProxy>(*shipDbView));
  queue->addListener(std::make_unique<ViewConsumerProxy>(*shopView));
  queue->addListener(std::make_unique<ViewConsumerProxy>(*systemView));
  queue->addListener(std::make_unique<ViewConsumerProxy>(*serverView));
  queue->addListener(std::make_unique<ViewConsumerProxy>(*resourceView));
}

void Views::reset()
{
  playerView->reset();
  shipView->reset();
  shipDbView->reset();
  shopView->reset();
  systemView->reset();
  serverView->reset();
  resourceView->reset();
}

} // namespace pge
