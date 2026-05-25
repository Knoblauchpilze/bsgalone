
#pragma once

#include "CoreObject.hh"
#include "EntityRegistry.hh"
#include "ForRunningSimulation.hh"
#include "IGame.hh"

namespace bsgalone::client {

class Game : public IGame, public ::core::CoreObject
{
  public:
  Game(core::EntityRegistryShPtr entityRegistry, core::ForRunningSimulationPtr coordinator);
  ~Game() override = default;

  void onSystemDataReceived(const SystemData &data) override;
  void update(const float elapsedSeconds) override;
  void reset() override;

  private:
  core::EntityRegistryShPtr m_entityRegistry{};
  core::ForRunningSimulationPtr m_coordinator{};
  chrono::ITimeManagerPtr m_timeManager{};

  void createEntities(const SystemData &data);
  void createAsteroids(const std::vector<core::Asteroid> &asteroids);
};

} // namespace bsgalone::client
