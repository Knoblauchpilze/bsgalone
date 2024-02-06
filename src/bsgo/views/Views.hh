
#pragma once

#include "PlayerView.hh"
#include "ResourceView.hh"
#include "ServerView.hh"
#include "ShipDbView.hh"
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
  ShipDbViewShPtr shipDbView{};
  ShopViewShPtr shopView{};
  SystemViewShPtr systemView{};
  ServerViewShPtr serverView{};
  ResourceViewShPtr resourceView{};
};

struct ViewsConfig
{
  Repositories repositories{};
  CoordinatorShPtr coordinator{};
  IMessageQueue *const internalMessageQueue{};
  IMessageQueue *const outputMessageQueue{};
};

auto createViews(const ViewsConfig &config, const DatabaseEntityMapper &entityMapper) -> Views;

} // namespace bsgo
