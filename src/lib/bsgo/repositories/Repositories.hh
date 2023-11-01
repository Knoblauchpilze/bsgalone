
#pragma once

#include "AsteroidLootRepository.hh"
#include "AsteroidRepository.hh"
#include "ComputerRepository.hh"
#include "OutpostRepository.hh"
#include "PlayerComputerRepository.hh"
#include "PlayerRepository.hh"
#include "PlayerResourceRepository.hh"
#include "PlayerShipRepository.hh"
#include "PlayerWeaponRepository.hh"
#include "ShipComputerRepository.hh"
#include "ShipWeaponRepository.hh"
#include "SystemRepository.hh"
#include "WeaponRepository.hh"

namespace bsgo {

struct Repositories
{
  AsteroidLootRepositoryShPtr asteroidLootRepository{std::make_shared<AsteroidLootRepository>()};
  AsteroidRepositoryShPtr asteroidRepository{std::make_shared<AsteroidRepository>()};
  ComputerRepositoryShPtr computerRepository{std::make_shared<ComputerRepository>()};
  OutpostRepositoryShPtr outpostRepository{std::make_shared<OutpostRepository>()};
  PlayerRepositoryShPtr playerRepository{std::make_shared<PlayerRepository>()};
  PlayerResourceRepositoryShPtr playerResourceRepository{
    std::make_shared<PlayerResourceRepository>()};
  PlayerShipRepositoryShPtr playerShipRepository{std::make_shared<PlayerShipRepository>()};
  SystemRepositoryShPtr systemRepository{std::make_shared<SystemRepository>()};
  WeaponRepositoryShPtr weaponRepository{std::make_shared<WeaponRepository>()};
  PlayerWeaponRepositoryShPtr playerWeaponRepository{std::make_shared<PlayerWeaponRepository>()};
  ShipWeaponRepositoryShPtr shipWeaponRepository{std::make_shared<ShipWeaponRepository>()};
  PlayerComputerRepositoryShPtr playerComputerRepository{
    std::make_shared<PlayerComputerRepository>()};
  ShipComputerRepositoryShPtr shipComputerRepository{std::make_shared<ShipComputerRepository>()};
};

} // namespace bsgo
