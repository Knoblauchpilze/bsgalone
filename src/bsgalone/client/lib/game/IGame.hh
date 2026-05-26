
#pragma once

#include "Asteroid.hh"
#include "IBoundingBox.hh"
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

  /// @brief - Used to ask the game to reset the simulation. This can typically be called
  /// when the user jumps from one system to the next or when a docking operation happens.
  virtual void reset() = 0;

  /// @brief - Used to request the asteroids that are within the bounding box.
  /// @param box - the bounding box describing the area in which asteroids should be
  /// @return - the list of asteroids within the bounding box
  virtual auto getAsteroidsWithin(const core::IBoundingBox &bbox) const
    -> std::vector<core::Asteroid>
    = 0;
};

using IGameShPtr = std::shared_ptr<IGame>;

} // namespace bsgalone::client
