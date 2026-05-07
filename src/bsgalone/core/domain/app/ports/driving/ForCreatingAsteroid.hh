
#pragma once

#include "Asteroid.hh"
#include "Uuid.hh"

namespace bsgalone::core {

class ForCreatingAsteroid
{
  public:
  ForCreatingAsteroid()          = default;
  virtual ~ForCreatingAsteroid() = default;

  virtual auto create(const Asteroid &asteroid) -> Uuid = 0;
};

} // namespace bsgalone::core
