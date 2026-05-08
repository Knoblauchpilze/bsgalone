
#pragma once

#include "Uuid.hh"

namespace bsgalone::server {

class ForInitializingSystem
{
  public:
  ForInitializingSystem()          = default;
  virtual ~ForInitializingSystem() = default;

  virtual void initializeSystem(const core::Uuid systemDbId) = 0;
};

} // namespace bsgalone::server
