
#pragma once

#include "Uuid.hh"

namespace bsgalone::core {

class ForInitializingSystem
{
  public:
  ForInitializingSystem()          = default;
  virtual ~ForInitializingSystem() = default;

  virtual void initializeSystem(const Uuid systemDbId) = 0;
};

} // namespace bsgalone::core
