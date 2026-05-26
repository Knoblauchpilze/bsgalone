
#pragma once

#include "CoreObject.hh"
#include "EntityRegistry.hh"
#include "ForCreatingAsteroid.hh"
#include "ForFetchingAsteroid.hh"
#include "ForRunningSimulation.hh"
#include "IGame.hh"

namespace bsgalone::client {

class GameBuilder;

class Game : public IGame, public ::core::CoreObject
{
  public:
  ~Game() override = default;

  void onSystemDataReceived(const SystemData &data) override;
  void update(const float elapsedSeconds) override;
  void reset() override;

  auto getAsteroidsWithin(const core::IBoundingBox &bbox) const
    -> std::vector<core::Asteroid> override;

  private:
  core::EntityRegistryShPtr m_entityRegistry{};
  core::ForRunningSimulationPtr m_coordinator{};
  core::ForCreatingAsteroidPtr m_asteroidCreator{};
  core::ForFetchingAsteroidPtr m_asteroidFetcher{};

  chrono::ITimeManagerPtr m_timeManager{};

  Game();

  void createEntities(const SystemData &data);
  void createAsteroids(const std::vector<core::Asteroid> &asteroids);

  friend class GameBuilder;
};

} // namespace bsgalone::client
