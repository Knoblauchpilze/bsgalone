
#pragma once

#include "TickData.hh"
#include <memory>

namespace bsgalone::core {

class IEcsCoordinator
{
  public:
  virtual ~IEcsCoordinator() = default;

  /// @brief - Defines an interface to update the systems registered in the ECS. This
  /// allows to abstract the simulation process and propose a general processing
  /// framework that works no matter the details of the ECS implementation.
  /// @param data - information about the current tick of the simulation
  virtual void update(const chrono::TickData &data) = 0;
};

using IEcsCoordinatorPtr = std::unique_ptr<IEcsCoordinator>;

} // namespace bsgalone::core
