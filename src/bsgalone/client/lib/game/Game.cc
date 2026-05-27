
#include "Game.hh"
#include "EcsCoordinator.hh"
#include "TimeManager.hh"

namespace bsgalone::client {

Game::Game()
  : IGame()
  , ::core::CoreObject("game")
{}

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

auto Game::getAsteroidsWithin(const core::IBoundingBox &bbox) const -> std::vector<core::Asteroid>
{
  return m_asteroidFetcher->getAsteroidsWithin(bbox);
}

void Game::createEntities(const SystemData &data)
{
  createAsteroids(data.asteroids);
}

void Game::createAsteroids(const std::vector<core::Asteroid> &asteroids)
{
  for (const auto &asteroid : asteroids)
  {
    m_asteroidCreator->create(asteroid);
  }
}

} // namespace bsgalone::client
