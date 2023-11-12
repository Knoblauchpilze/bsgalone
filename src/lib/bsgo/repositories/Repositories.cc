
#include "Repositories.hh"

namespace bsgo {

auto createRepositories(const DbConnectionShPtr &dbConnection) -> Repositories
{
  Repositories out;

  out.asteroidLootRepository   = std::make_shared<AsteroidLootRepository>(dbConnection);
  out.asteroidRepository       = std::make_shared<AsteroidRepository>(dbConnection);
  out.computerPriceRepository  = std::make_shared<ComputerPriceRepository>(dbConnection);
  out.computerRepository       = std::make_shared<ComputerRepository>(dbConnection);
  out.outpostRepository        = std::make_shared<OutpostRepository>(dbConnection);
  out.playerComputerRepository = std::make_shared<PlayerComputerRepository>(dbConnection);
  out.playerRepository         = std::make_shared<PlayerRepository>(dbConnection);
  out.playerResourceRepository = std::make_shared<PlayerResourceRepository>(dbConnection);
  out.playerShipRepository     = std::make_shared<PlayerShipRepository>(dbConnection);
  out.playerWeaponRepository   = std::make_shared<PlayerWeaponRepository>(dbConnection);
  out.resourceRepository       = std::make_shared<ResourceRepository>(dbConnection);
  out.shipComputerRepository   = std::make_shared<ShipComputerRepository>(dbConnection);
  out.shipWeaponRepository     = std::make_shared<ShipWeaponRepository>(dbConnection);
  out.systemRepository         = std::make_shared<SystemRepository>(dbConnection);
  out.weaponPriceRepository    = std::make_shared<WeaponPriceRepository>(dbConnection);
  out.weaponRepository         = std::make_shared<WeaponRepository>(dbConnection);

  return out;
}

} // namespace bsgo