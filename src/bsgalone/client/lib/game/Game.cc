
#include "Game.hh"
#include "EcsCoordinator.hh"
#include "TimeManager.hh"

namespace bsgalone::client {

Game::Game(core::ForRunningSimulationPtr coordinator)
  : IGame()
  , ::core::CoreObject("game")
  , m_coordinator(std::move(coordinator))
{
  if (m_coordinator == nullptr)
  {
    throw std::invalid_argument("Expected non null coordinator");
  }
}

void Game::onSystemDataReceived(const SystemData &data)
{
  m_timeManager = std::make_unique<chrono::TimeManager>(data.currentTick, data.step);
}

void Game::update(const float elapsedSeconds)
{
  if (m_timeManager == nullptr)
  {
    throw std::invalid_argument("System data has not been received");
  }

  const auto elapsed = chrono::Duration{
    .unit    = chrono::Unit::SECONDS,
    .elapsed = elapsedSeconds,
  };

  const auto data = m_timeManager->tick(elapsed);
  m_coordinator->update(data);
}

void Game::reset()
{
  m_timeManager.reset();
  m_coordinator->clear();
}

} // namespace bsgalone::client
