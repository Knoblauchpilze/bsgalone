
#pragma once

#include "AccountRepository.hh"
#include "AiBehaviorRepository.hh"
#include "AsteroidLootRepository.hh"
#include "AsteroidRepository.hh"
#include "ComputerPriceRepository.hh"
#include "ComputerRepository.hh"
#include "DbConnection.hh"
#include "OutpostRepository.hh"
#include "PlayerComputerRepository.hh"
#include "PlayerRepository.hh"
#include "PlayerResourceRepository.hh"
#include "PlayerRoleRepository.hh"
#include "PlayerShipRepository.hh"
#include "PlayerWeaponRepository.hh"
#include "ResourceRepository.hh"
#include "ShipComputerRepository.hh"
#include "ShipPriceRepository.hh"
#include "ShipRepository.hh"
#include "ShipWeaponRepository.hh"
#include "SystemOutpostRepository.hh"
#include "SystemRepository.hh"
#include "TickRepository.hh"
#include "WeaponPriceRepository.hh"
#include "WeaponRepository.hh"

namespace bsgo {

struct Repositories
{
  Repositories();
  ~Repositories() = default;

  AccountRepositoryShPtr accountRepository{};
  AiBehaviorRepositoryShPtr aiBehaviorRepository{};
  AsteroidLootRepositoryShPtr asteroidLootRepository{};
  AsteroidRepositoryShPtr asteroidRepository{};
  ComputerPriceRepositoryShPtr computerPriceRepository{};
  ComputerRepositoryShPtr computerRepository{};
  OutpostRepositoryShPtr outpostRepository{};
  PlayerComputerRepositoryShPtr playerComputerRepository{};
  PlayerRepositoryShPtr playerRepository{};
  PlayerResourceRepositoryShPtr playerResourceRepository{};
  PlayerRoleRepositoryShPtr playerRoleRepository{};
  PlayerShipRepositoryShPtr playerShipRepository{};
  PlayerWeaponRepositoryShPtr playerWeaponRepository{};
  ResourceRepositoryShPtr resourceRepository{};
  ShipComputerRepositoryShPtr shipComputerRepository{};
  ShipPriceRepositoryShPtr shipPriceRepository{};
  ShipRepositoryShPtr shipRepository{};
  ShipWeaponRepositoryShPtr shipWeaponRepository{};
  SystemOutpostRepositoryShPtr systemOutpostRepository{};
  SystemRepositoryShPtr systemRepository{};
  TickRepositoryShPtr tickRepository{};
  WeaponPriceRepositoryShPtr weaponPriceRepository{};
  WeaponRepositoryShPtr weaponRepository{};

  DbConnectionShPtr dbConnection{};
};

} // namespace bsgo
