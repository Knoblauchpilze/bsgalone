
#include "Views.hh"
#include "ViewConsumerProxy.hh"

namespace pge {

auto createViews(const ViewsConfig &config, const bsgo::DatabaseEntityMapper &entityMapper) -> Views
{
  Views out{};

  out.shipView     = std::make_shared<ShipView>(config.coordinator,
                                            config.repositories,
                                            config.outputMessageQueue);
  out.shipDbView   = std::make_shared<ShipDbView>(config.repositories,
                                                config.internalMessageQueue,
                                                config.outputMessageQueue);
  out.systemView   = std::make_shared<SystemView>(config.coordinator, entityMapper);
  out.playerView   = std::make_shared<PlayerView>(config.repositories, config.outputMessageQueue);
  out.shopView     = std::make_shared<ShopView>(config.repositories);
  out.serverView   = std::make_shared<ServerView>(config.gameSession, config.repositories);
  out.resourceView = std::make_shared<ResourceView>(config.repositories);

  return out;
}

namespace {
void registerViewToQueue(AbstractView &view,
                         const std::unordered_set<bsgo::MessageType> &relevantMessageTypes,
                         bsgo::IMessageQueue *const queue)
{
  queue->addListener(std::make_unique<ViewConsumerProxy>(view, relevantMessageTypes));
}
} // namespace

void registerViews(const Views &views, bsgo::IMessageQueue *const queue)
{
  // TODO: specify the right kind of messages
  registerViewToQueue(*views.playerView, {}, queue);
  registerViewToQueue(*views.shipView, {}, queue);
  registerViewToQueue(*views.shipDbView, {}, queue);
  registerViewToQueue(*views.shopView, {}, queue);
  registerViewToQueue(*views.systemView, {}, queue);
  registerViewToQueue(*views.serverView, {}, queue);
  registerViewToQueue(*views.resourceView, {}, queue);
}

} // namespace pge
