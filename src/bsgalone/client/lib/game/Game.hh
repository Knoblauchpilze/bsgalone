
#pragma once

#include "CoreObject.hh"
#include "ForRunningSimulation.hh"
#include "IGame.hh"

namespace bsgalone::client {

class Game : public IGame, public ::core::CoreObject
{
  public:
  Game(core::ForRunningSimulationPtr coordinator);
  ~Game() override = default;

  void onSystemDataReceived(const SystemData &data) override;
  void update(const float elapsedSeconds) override;
  void reset() override;

  private:
  core::ForRunningSimulationPtr m_coordinator{};
  chrono::ITimeManagerPtr m_timeManager{};
};

} // namespace bsgalone::client
