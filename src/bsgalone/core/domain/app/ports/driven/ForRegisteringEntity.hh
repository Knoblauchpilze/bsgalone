
#pragma once

#include "Asteroid.hh"
#include <memory>

namespace bsgalone::core {

class ForRegisteringEntity
{
  public:
  virtual ~ForRegisteringEntity() = default;

  virtual void registerAsteroid(const Asteroid &asteroid) = 0;
};

using ForRegisteringEntityShPtr = std::shared_ptr<ForRegisteringEntity>;

} // namespace bsgalone::core
