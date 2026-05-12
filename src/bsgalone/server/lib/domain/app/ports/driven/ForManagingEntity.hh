
#pragma once

#include "Asteroid.hh"
#include <memory>
#include <vector>

namespace bsgalone::server {

class ForManagingEntity
{
  public:
  ForManagingEntity()          = default;
  virtual ~ForManagingEntity() = default;

  virtual auto createAsteroid(const core::Asteroid &asteroid) -> core::Uuid = 0;
};

using ForManagingEntityShPtr = std::shared_ptr<ForManagingEntity>;

} // namespace bsgalone::server
