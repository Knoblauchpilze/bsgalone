
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
#include "GameSession.hh"
#include "IMessageQueue.hh"
#include "Repositories.hh"

namespace pge {

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
  GameSessionShPtr gameSession{};
  bsgo::Repositories repositories{};
  bsgo::CoordinatorShPtr coordinator{};
  bsgo::IMessageQueue *const internalMessageQueue{};
  bsgo::IMessageQueue *const outputMessageQueue{};
};

auto createViews(const ViewsConfig &config, const bsgo::DatabaseEntityMapper &entityMapper)
  -> Views;

} // namespace pge
