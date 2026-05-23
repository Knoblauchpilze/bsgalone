
#pragma once

#include "ITimeManager.hh"
#include "SystemData.hh"
#include <memory>

namespace bsgalone::client {

class IGame
{
  public:
  IGame()          = default;
  virtual ~IGame() = default;

  /// @brief - Used to notify the game that the current system's data has been received.
  /// This data should be used to simulate the ECS supporting the game.
  /// @param data - the system's data coming from the server
  virtual void onSystemDataReceived(const SystemData &data) = 0;

  /// @brief - Moves the simulation one step forward in time.
  /// @param elapsedSeconds - defines how many real world seconds passed since the last
  /// call to this function
  virtual void update(const float elapsedSeconds) = 0;
};

using IGameShPtr = std::shared_ptr<IGame>;

} // namespace bsgalone::client
