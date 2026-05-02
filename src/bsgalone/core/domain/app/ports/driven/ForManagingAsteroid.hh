
#pragma once

#include "Asteroid.hh"
#include <memory>
#include <vector>

namespace bsgalone::core {

class ForManagingAsteroid
{
  public:
  ForManagingAsteroid()          = default;
  virtual ~ForManagingAsteroid() = default;

  virtual auto findAllBySystem(const Uuid systemDbId) const -> std::vector<Asteroid> = 0;
};

using ForManagingAsteroidShPtr = std::shared_ptr<ForManagingAsteroid>;

} // namespace bsgalone::core
