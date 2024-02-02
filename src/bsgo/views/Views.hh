
#pragma once

#include "PlayerView.hh"
#include "ResourceView.hh"
#include "ServerView.hh"
#include "ShipView.hh"
#include "ShopView.hh"
#include "SystemView.hh"

#include "Coordinator.hh"
#include "DatabaseEntityMapper.hh"
#include "IMessageQueue.hh"
#include "Repositories.hh"

namespace bsgo {

struct Views
{
  PlayerViewShPtr playerView{};
  ShipViewShPtr shipView{};
  ShopViewShPtr shopView{};
  SystemViewShPtr systemView{};
  ServerViewShPtr serverView{};
  ResourceViewShPtr resourceView{};
};

auto createViews(const CoordinatorShPtr &coordinator,
                 const Repositories &repositories,
                 const DatabaseEntityMapper &entityMapper,
                 IMessageQueue *const messageQueue) -> Views;

} // namespace bsgo
