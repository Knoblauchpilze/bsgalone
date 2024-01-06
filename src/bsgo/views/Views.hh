
#pragma once

#include "PlayerView.hh"
#include "ResourceView.hh"
#include "ServerView.hh"
#include "ShipView.hh"
#include "ShopView.hh"
#include "SystemView.hh"

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

} // namespace bsgo
