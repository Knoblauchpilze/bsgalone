
#pragma once

#include "LoginView.hh"
#include "PlayerView.hh"
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
  LoginViewShPtr loginView{};
};

} // namespace bsgo
