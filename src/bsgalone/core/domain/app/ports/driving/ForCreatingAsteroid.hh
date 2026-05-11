
#pragma once

#include "Asteroid.hh"
#include "Uuid.hh"
#include <memory>

namespace bsgalone::core {

class ForCreatingAsteroid
{
  public:
  ForCreatingAsteroid()          = default;
  virtual ~ForCreatingAsteroid() = default;

  virtual auto create(const Asteroid &asteroid) -> Uuid = 0;
};

using ForCreatingAsteroidPtr = std::unique_ptr<ForCreatingAsteroid>;

} // namespace bsgalone::core
