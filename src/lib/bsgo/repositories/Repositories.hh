
#pragma once

#include "AsteroidRepository.hh"
#include "PlayerRepository.hh"
#include "PlayerShipRepository.hh"
#include "ShipRepository.hh"
#include "SystemRepository.hh"

namespace bsgo {

struct Repositories
{
  Uuid playerId{};

  AsteroidRepositoryShPtr asteroid{};
  ShipRepositoryShPtr ship{};
  PlayerRepositoryShPtr player{};
  SystemRepositoryShPtr system{};
  PlayerShipRepositoryShPtr playerShip{};
};

} // namespace bsgo
