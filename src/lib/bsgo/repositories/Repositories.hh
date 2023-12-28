
#pragma once

#include "AsteroidLootRepository.hh"
#include "AsteroidRepository.hh"
#include "ComputerPriceRepository.hh"
#include "ComputerRepository.hh"
#include "DbConnection.hh"
#include "OutpostRepository.hh"
#include "PlayerComputerRepository.hh"
#include "PlayerRepository.hh"
#include "PlayerResourceRepository.hh"
#include "PlayerShipRepository.hh"
#include "PlayerWeaponRepository.hh"
#include "ResourceRepository.hh"
#include "ShipComputerRepository.hh"
#include "ShipPriceRepository.hh"
#include "ShipRepository.hh"
#include "ShipWeaponRepository.hh"
#include "SystemOutpostRepository.hh"
#include "SystemRepository.hh"
#include "WeaponPriceRepository.hh"
#include "WeaponRepository.hh"

namespace bsgo {

struct Repositories
{
  AsteroidLootRepositoryShPtr asteroidLootRepository{};
  AsteroidRepositoryShPtr asteroidRepository{};
  ComputerPriceRepositoryShPtr computerPriceRepository{};
  ComputerRepositoryShPtr computerRepository{};
  OutpostRepositoryShPtr outpostRepository{};
  PlayerComputerRepositoryShPtr playerComputerRepository{};
  PlayerRepositoryShPtr playerRepository{};
  PlayerResourceRepositoryShPtr playerResourceRepository{};
  PlayerShipRepositoryShPtr playerShipRepository{};
  PlayerWeaponRepositoryShPtr playerWeaponRepository{};
  ResourceRepositoryShPtr resourceRepository{};
  ShipPriceRepositoryShPtr shipPriceRepository{};
  ShipRepositoryShPtr shipRepository{};
  ShipComputerRepositoryShPtr shipComputerRepository{};
  ShipWeaponRepositoryShPtr shipWeaponRepository{};
  SystemRepositoryShPtr systemRepository{};
  WeaponPriceRepositoryShPtr weaponPriceRepository{};
  WeaponRepositoryShPtr weaponRepository{};
  SystemOutpostRepositoryShPtr systemOutpostRepository{};
};

auto createRepositories(const DbConnectionShPtr &dbConnection) -> Repositories;

} // namespace bsgo
