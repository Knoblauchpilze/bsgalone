
#pragma once

#include "PlayerView.hh"
#include "ServerView.hh"
#include "ShipView.hh"
#include "ShopView.hh"
#include "SystemView.hh"

namespace bsgo {

struct Views
{
  PlayerViewShPtr playerView{};
  ServerViewShPtr serverView{};
  ShipViewShPtr shipView{};
  ShopViewShPtr shopView{};
  SystemViewShPtr systemView{};
};

} // namespace bsgo
