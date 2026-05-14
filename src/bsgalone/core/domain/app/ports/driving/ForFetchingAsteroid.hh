
#pragma once

#include "Asteroid.hh"
#include "Uuid.hh"
#include <memory>
#include <vector>

namespace bsgalone::core {

class ForFetchingAsteroid
{
  public:
  ForFetchingAsteroid()          = default;
  virtual ~ForFetchingAsteroid() = default;

  virtual auto getAllAsteroids() const -> std::vector<Asteroid> = 0;
};

using ForFetchingAsteroidPtr = std::unique_ptr<ForFetchingAsteroid>;

} // namespace bsgalone::core
