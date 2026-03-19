
#pragma once

#include "GameView.hh"

#include "Coordinator.hh"
#include "DatabaseEntityMapper.hh"
#include "GameSession.hh"
#include "IMessageQueue.hh"

namespace bsgalone::client {

struct Views
{
  GameViewShPtr gameView{};

  void connectToQueue(core::IMessageQueue *const queue);
  void reset();
};

struct ViewsConfig
{
  GameSessionShPtr gameSession{};
  core::CoordinatorShPtr coordinator{};
  core::IMessageQueueShPtr internalMessageQueue{};
  core::IMessageQueueShPtr outputMessageQueue{};
};

auto createViews(const ViewsConfig &config, const core::DatabaseEntityMapper &entityMapper)
  -> Views;

} // namespace bsgalone::client
