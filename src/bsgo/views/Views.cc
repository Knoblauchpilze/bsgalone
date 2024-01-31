
#include "Views.hh"

namespace bsgo {

auto createViews(const CoordinatorShPtr &coordinator,
                 const Repositories &repositories,
                 IMessageQueue *const messageQueue) -> Views
{
  Views out{};

  out.shipView     = std::make_shared<bsgo::ShipView>(coordinator, repositories, messageQueue);
  out.systemView   = std::make_shared<bsgo::SystemView>(coordinator, repositories, messageQueue);
  out.playerView   = std::make_shared<bsgo::PlayerView>(coordinator, repositories, messageQueue);
  out.shopView     = std::make_shared<bsgo::ShopView>(coordinator, repositories, messageQueue);
  out.serverView   = std::make_shared<bsgo::ServerView>(coordinator, repositories, messageQueue);
  out.resourceView = std::make_shared<bsgo::ResourceView>(coordinator, repositories, messageQueue);

  return out;
}

} // namespace bsgo
