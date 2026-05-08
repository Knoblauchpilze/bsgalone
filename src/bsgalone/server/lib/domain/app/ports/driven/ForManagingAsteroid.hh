
#pragma once

#include "Asteroid.hh"
#include <memory>
#include <vector>

namespace bsgalone::server {

class ForManagingAsteroid
{
  public:
  ForManagingAsteroid()          = default;
  virtual ~ForManagingAsteroid() = default;

  virtual auto findAllBySystem(const core::Uuid systemDbId) const -> std::vector<core::Asteroid>
    = 0;
};

using ForManagingAsteroidShPtr = std::shared_ptr<ForManagingAsteroid>;

} // namespace bsgalone::server
