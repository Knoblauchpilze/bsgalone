
#include "Views.hh"
#include "ViewConsumerProxy.hh"

namespace pge {

auto createViews(const ViewsConfig &config, const bsgo::DatabaseEntityMapper &entityMapper) -> Views
{
  Views out{};

  out.shipView     = std::make_shared<ShipView>(config.coordinator, config.outputMessageQueue);
  out.shipDbView   = std::make_shared<ShipDbView>(config.repositories,
                                                config.gameSession,
                                                config.internalMessageQueue,
                                                config.outputMessageQueue);
  out.systemView   = std::make_shared<SystemView>(config.coordinator, entityMapper);
  out.playerView   = std::make_shared<PlayerView>(config.repositories,
                                                config.gameSession,
                                                config.outputMessageQueue);
  out.shopView     = std::make_shared<ShopView>(config.repositories);
  out.serverView   = std::make_shared<ServerView>(config.gameSession);
  out.resourceView = std::make_shared<ResourceView>();

  return out;
}

namespace {
void registerViewToQueue(AbstractView &view,
                         const std::unordered_set<bsgo::MessageType> &relevantMessageTypes,
                         bsgo::IMessageQueue *const queue)
{
  if (!relevantMessageTypes.empty())
  {
    queue->addListener(std::make_unique<ViewConsumerProxy>(view, relevantMessageTypes));
  }
}
} // namespace

void registerViews(const Views &views, bsgo::IMessageQueue *const queue)
{
  std::unordered_set<bsgo::MessageType> messageTypes{};

  // TODO: specify the right kind of messages
  messageTypes = {bsgo::MessageType::PLAYER_COMPUTER_LIST,
                  bsgo::MessageType::PLAYER_RESOURCE_LIST,
                  bsgo::MessageType::PLAYER_SHIP_LIST,
                  bsgo::MessageType::PLAYER_WEAPON_LIST};
  registerViewToQueue(*views.playerView, messageTypes, queue);

  messageTypes = {bsgo::MessageType::SYSTEM_LIST};
  registerViewToQueue(*views.shipView, messageTypes, queue);

  messageTypes = {};
  registerViewToQueue(*views.shipDbView, messageTypes, queue);
  registerViewToQueue(*views.shopView, messageTypes, queue);
  registerViewToQueue(*views.systemView, messageTypes, queue);

  messageTypes = {bsgo::MessageType::SYSTEM_LIST};
  registerViewToQueue(*views.serverView, messageTypes, queue);

  messageTypes = {bsgo::MessageType::RESOURCE_LIST};
  registerViewToQueue(*views.resourceView, messageTypes, queue);
}

} // namespace pge
