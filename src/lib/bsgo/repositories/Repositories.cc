
#include "Repositories.hh"

namespace bsgo {

auto createRepositories(const DbConnectionShPtr &dbConnection) -> Repositories
{
  Repositories out{};

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
  out.shipPriceRepository      = std::make_shared<ShipPriceRepository>(dbConnection);
  out.shipRepository           = std::make_shared<ShipRepository>(dbConnection);
  out.shipComputerRepository   = std::make_shared<ShipComputerRepository>(dbConnection);
  out.shipWeaponRepository     = std::make_shared<ShipWeaponRepository>(dbConnection);
  out.systemRepository         = std::make_shared<SystemRepository>(dbConnection);
  out.weaponPriceRepository    = std::make_shared<WeaponPriceRepository>(dbConnection);
  out.weaponRepository         = std::make_shared<WeaponRepository>(dbConnection);
  out.systemOutpostRepository  = std::make_shared<SystemOutpostRepository>(dbConnection);

  out.asteroidLootRepository->initialize();
  out.asteroidRepository->initialize();
  out.computerPriceRepository->initialize();
  out.computerRepository->initialize();
  out.outpostRepository->initialize();
  out.playerComputerRepository->initialize();
  out.playerRepository->initialize();
  out.playerResourceRepository->initialize();
  out.playerShipRepository->initialize();
  out.playerWeaponRepository->initialize();
  out.resourceRepository->initialize();
  out.shipPriceRepository->initialize();
  out.shipRepository->initialize();
  out.shipComputerRepository->initialize();
  out.shipWeaponRepository->initialize();
  out.systemRepository->initialize();
  out.weaponPriceRepository->initialize();
  out.weaponRepository->initialize();
  out.systemOutpostRepository->initialize();

  return out;
}

} // namespace bsgo
