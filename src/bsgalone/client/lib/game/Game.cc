
#include "Game.hh"
#include "EcsCoordinator.hh"
#include "TimeManager.hh"

namespace bsgalone::client {

Game::Game(core::EntityRegistryShPtr entityRegistry, core::ForRunningSimulationPtr coordinator)
  : IGame()
  , ::core::CoreObject("game")
  , m_entityRegistry(std::move(entityRegistry))
  , m_coordinator(std::move(coordinator))
{
  if (m_entityRegistry == nullptr)
  {
    throw std::invalid_argument("Expected non null entity registry");
  }
  if (m_coordinator == nullptr)
  {
    throw std::invalid_argument("Expected non null coordinator");
  }
}

void Game::onSystemDataReceived(const SystemData &data)
{
  m_timeManager = std::make_unique<chrono::TimeManager>(data.currentTick, data.step);
  createEntities(data);
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
  m_entityRegistry->clear();
}

void Game::createEntities(const SystemData &data)
{
  createAsteroids(data.asteroids);
}

void Game::createAsteroids(const std::vector<core::Asteroid> &asteroids)
{
  for (const auto &asteroid : asteroids)
  {
    // TODO: Handle asteroid creation
    info("should create asteroid " + asteroid.dbId.str());
  }
}

} // namespace bsgalone::client
