
#pragma once

#include "AsteroidRepository.hh"
#include "ShipRepository.hh"
#include "SystemRepository.hh"

namespace bsgo {

struct Repositories
{
  AsteroidRepositoryShPtr asteroid{};
  ShipRepositoryShPtr ship{};
  SystemRepositoryShPtr system{};
};

} // namespace bsgo
