
#pragma once

#include "AsteroidRepository.hh"
#include "PlayerRepository.hh"
#include "ShipRepository.hh"
#include "SystemRepository.hh"

namespace bsgo {

struct Repositories
{
  AsteroidRepositoryShPtr asteroid{};
  ShipRepositoryShPtr ship{};
  PlayerRepositoryShPtr player{};
  SystemRepositoryShPtr system{};
};

} // namespace bsgo
