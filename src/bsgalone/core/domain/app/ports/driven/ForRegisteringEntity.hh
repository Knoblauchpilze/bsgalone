
#pragma once

#include "Asteroid.hh"
#include "EntityId.hh"
#include <memory>

namespace bsgalone::core {

class ForRegisteringEntity
{
  public:
  virtual ~ForRegisteringEntity() = default;

  virtual auto registerAsteroid(const Asteroid &asteroid) -> EntityId = 0;
};

using ForRegisteringEntityShPtr = std::shared_ptr<ForRegisteringEntity>;

} // namespace bsgalone::core
