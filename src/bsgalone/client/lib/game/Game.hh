
#pragma once

#include "CoreObject.hh"
#include "ForRunningSimulation.hh"
#include "ITimeManager.hh"
#include "SystemData.hh"
#include <memory>

namespace bsgalone::client {

class Game : public ::core::CoreObject
{
  public:
  Game(core::ForRunningSimulationPtr coordinator);
  ~Game() override = default;

  void onSystemDataReceived(const SystemData &data);

  /// @brief - Moves the simulation one step forward in time.
  /// @param elapsedSeconds - defines how many real world seconds passed since the last
  /// call to this function
  void update(const float elapsedSeconds);

  private:
  core::ForRunningSimulationPtr m_coordinator{};
  chrono::ITimeManagerPtr m_timeManager{};
};

using GamePtr = std::unique_ptr<Game>;

} // namespace bsgalone::client
