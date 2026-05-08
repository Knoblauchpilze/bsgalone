
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

  virtual void createAsteroid(const core::Asteroid &asteroid) = 0;
};

using ForManagingEntityShPtr = std::shared_ptr<ForManagingEntity>;

} // namespace bsgalone::server
