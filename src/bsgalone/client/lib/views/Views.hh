
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

namespace bsgalone::client {

struct Views
{
  PlayerViewShPtr playerView{};
  ShipViewShPtr shipView{};
  ShipDbViewShPtr shipDbView{};
  ShopViewShPtr shopView{};
  SystemViewShPtr systemView{};
  ServerViewShPtr serverView{};
  ResourceViewShPtr resourceView{};

  void connectToQueue(core::IMessageQueue *const queue);
  void reset();
};

struct ViewsConfig
{
  GameSessionShPtr gameSession{};
  core::CoordinatorShPtr coordinator{};
  core::IMessageQueue *const internalMessageQueue{};
  core::IMessageQueue *const outputMessageQueue{};
};

auto createViews(const ViewsConfig &config, const core::DatabaseEntityMapper &entityMapper)
  -> Views;

} // namespace bsgalone::client
