
#include "Views.hh"

namespace bsgo {

auto createViews(const ViewsConfig &config, const DatabaseEntityMapper &entityMapper) -> Views
{
  Views out{};

  out.shipView     = std::make_shared<bsgo::ShipView>(config.coordinator,
                                                  config.repositories,
                                                  config.internalMessageQueue,
                                                  config.outputMessageQueue);
  out.systemView   = std::make_shared<bsgo::SystemView>(config.coordinator, entityMapper);
  out.playerView   = std::make_shared<bsgo::PlayerView>(config.repositories,
                                                      config.outputMessageQueue);
  out.shopView     = std::make_shared<bsgo::ShopView>(config.repositories);
  out.serverView   = std::make_shared<bsgo::ServerView>(config.repositories);
  out.resourceView = std::make_shared<bsgo::ResourceView>(config.repositories);

  return out;
}

} // namespace bsgo
