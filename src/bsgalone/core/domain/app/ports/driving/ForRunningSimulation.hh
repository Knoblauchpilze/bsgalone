
#pragma once

#include "TickData.hh"
#include <memory>

namespace bsgalone::core {

class ForRunningSimulation
{
  public:
  virtual ~ForRunningSimulation() = default;

  /// @brief - Defines an interface to update the systems registered in the ECS. This
  /// allows to abstract the simulation process and propose a general processing
  /// framework that works no matter the details of the ECS implementation.
  /// @param data - information about the current tick of the simulation
  virtual void update(const chrono::TickData &data) = 0;

  /// @brief - Defines an interface to clear all entities and components registered in
  /// the simulation. This method should not impact the systems: it should only change
  /// the content of the simulation.
  virtual void clear() = 0;
};

using ForRunningSimulationPtr = std::unique_ptr<ForRunningSimulation>;

} // namespace bsgalone::core
