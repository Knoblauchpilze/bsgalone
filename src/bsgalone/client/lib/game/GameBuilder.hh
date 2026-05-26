
#pragma once

#include "EntityRegistry.hh"
#include "ForCreatingAsteroid.hh"
#include "ForFetchingAsteroid.hh"
#include "ForRunningSimulation.hh"
#include "IGame.hh"

namespace bsgalone::client {

class GameBuilder
{
  public:
  GameBuilder()  = default;
  ~GameBuilder() = default;

  auto withEntityRegistry(core::EntityRegistryShPtr entityRegistry) -> GameBuilder &;
  auto withSimulationRunner(core::ForRunningSimulationPtr coordinator) -> GameBuilder &;
  auto withAsteroidCreator(core::ForCreatingAsteroidPtr creator) -> GameBuilder &;
  auto withAsteroidFetcher(core::ForFetchingAsteroidPtr fetcher) -> GameBuilder &;

  auto build() -> IGameShPtr;

  private:
  core::EntityRegistryShPtr m_entityRegistry{};
  core::ForRunningSimulationPtr m_coordinator{};
  core::ForCreatingAsteroidPtr m_asteroidCreator{};
  core::ForFetchingAsteroidPtr m_asteroidFetcher{};

  struct ptrEnabler;
  friend class ptrEnabler;

  void validate() const;
};

} // namespace bsgalone::client
