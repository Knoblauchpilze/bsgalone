
#pragma once

#include "EntityRegistry.hh"
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

  auto build() -> IGameShPtr;

  private:
  core::EntityRegistryShPtr m_entityRegistry{};
  core::ForRunningSimulationPtr m_coordinator{};

  struct ptrEnabler;
  friend class ptrEnabler;

  void validate() const;
};

} // namespace bsgalone::client
