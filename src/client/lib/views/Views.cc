
#include "Views.hh"

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
  out.serverView   = std::make_shared<ServerView>(config.repositories);
  out.resourceView = std::make_shared<ResourceView>(config.repositories);

  return out;
}

} // namespace pge
