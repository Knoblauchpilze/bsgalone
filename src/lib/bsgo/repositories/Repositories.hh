
#pragma once

#include "AsteroidLootRepository.hh"
#include "AsteroidRepository.hh"
#include "ComputerPriceRepository.hh"
#include "ComputerRepository.hh"
#include "OutpostRepository.hh"
#include "PlayerComputerRepository.hh"
#include "PlayerRepository.hh"
#include "PlayerResourceRepository.hh"
#include "PlayerShipRepository.hh"
#include "PlayerWeaponRepository.hh"
#include "ResourceRepository.hh"
#include "ShipComputerRepository.hh"
#include "ShipWeaponRepository.hh"
#include "SystemRepository.hh"
#include "WeaponPriceRepository.hh"
#include "WeaponRepository.hh"

namespace bsgo {

struct Repositories
{
  AsteroidLootRepositoryShPtr asteroidLootRepository{std::make_shared<AsteroidLootRepository>()};
  AsteroidRepositoryShPtr asteroidRepository{std::make_shared<AsteroidRepository>()};
  ComputerPriceRepositoryShPtr computerPriceRepository{std::make_shared<ComputerPriceRepository>()};
  ComputerRepositoryShPtr computerRepository{std::make_shared<ComputerRepository>()};
  OutpostRepositoryShPtr outpostRepository{std::make_shared<OutpostRepository>()};
  PlayerComputerRepositoryShPtr playerComputerRepository{
    std::make_shared<PlayerComputerRepository>()};
  PlayerRepositoryShPtr playerRepository{std::make_shared<PlayerRepository>()};
  PlayerResourceRepositoryShPtr playerResourceRepository{
    std::make_shared<PlayerResourceRepository>()};
  PlayerShipRepositoryShPtr playerShipRepository{std::make_shared<PlayerShipRepository>()};
  PlayerWeaponRepositoryShPtr playerWeaponRepository{std::make_shared<PlayerWeaponRepository>()};
  ResourceRepositoryShPtr resourceRepository{std::make_shared<ResourceRepository>()};
  ShipComputerRepositoryShPtr shipComputerRepository{std::make_shared<ShipComputerRepository>()};
  ShipWeaponRepositoryShPtr shipWeaponRepository{std::make_shared<ShipWeaponRepository>()};
  SystemRepositoryShPtr systemRepository{std::make_shared<SystemRepository>()};
  WeaponPriceRepositoryShPtr weaponPriceRepository{std::make_shared<WeaponPriceRepository>()};
  WeaponRepositoryShPtr weaponRepository{std::make_shared<WeaponRepository>()};
};

} // namespace bsgo
