
#pragma once

#include "System.hh"
#include <memory>
#include <vector>

namespace bsgalone::server {

class ForManagingSimulation
{
  public:
  ForManagingSimulation()          = default;
  virtual ~ForManagingSimulation() = default;

  virtual void start() = 0;
  virtual void stop()  = 0;
};

using ForManagingSimulationShPtr = std::shared_ptr<ForManagingSimulation>;

} // namespace bsgalone::server
