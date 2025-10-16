
#include "Repositories.hh"

namespace bsgo {

Repositories::Repositories()
{
  dbConnection = std::make_shared<DbConnection>();
  dbConnection->connect();

  asteroidLootRepository   = std::make_shared<AsteroidLootRepository>(dbConnection);
  asteroidRepository       = std::make_shared<AsteroidRepository>(dbConnection);
  computerPriceRepository  = std::make_shared<ComputerPriceRepository>(dbConnection);
  computerRepository       = std::make_shared<ComputerRepository>(dbConnection);
  outpostRepository        = std::make_shared<OutpostRepository>(dbConnection);
  playerComputerRepository = std::make_shared<PlayerComputerRepository>(dbConnection);
  playerRepository         = std::make_shared<PlayerRepository>(dbConnection);
  playerResourceRepository = std::make_shared<PlayerResourceRepository>(dbConnection);
  playerShipRepository     = std::make_shared<PlayerShipRepository>(dbConnection);
  playerWeaponRepository   = std::make_shared<PlayerWeaponRepository>(dbConnection);
  resourceRepository       = std::make_shared<ResourceRepository>(dbConnection);
  shipPriceRepository      = std::make_shared<ShipPriceRepository>(dbConnection);
  shipRepository           = std::make_shared<ShipRepository>(dbConnection);
  shipComputerRepository   = std::make_shared<ShipComputerRepository>(dbConnection);
  shipWeaponRepository     = std::make_shared<ShipWeaponRepository>(dbConnection);
  systemRepository         = std::make_shared<SystemRepository>(dbConnection);
  weaponPriceRepository    = std::make_shared<WeaponPriceRepository>(dbConnection);
  weaponRepository         = std::make_shared<WeaponRepository>(dbConnection);
  systemOutpostRepository  = std::make_shared<SystemOutpostRepository>(dbConnection);
  tickRepository           = std::make_shared<TickRepository>(dbConnection);

  asteroidLootRepository->initialize();
  asteroidRepository->initialize();
  computerPriceRepository->initialize();
  computerRepository->initialize();
  outpostRepository->initialize();
  playerComputerRepository->initialize();
  playerRepository->initialize();
  playerResourceRepository->initialize();
  playerShipRepository->initialize();
  playerWeaponRepository->initialize();
  resourceRepository->initialize();
  shipPriceRepository->initialize();
  shipRepository->initialize();
  shipComputerRepository->initialize();
  shipWeaponRepository->initialize();
  systemRepository->initialize();
  weaponPriceRepository->initialize();
  weaponRepository->initialize();
  systemOutpostRepository->initialize();
  tickRepository->initialize();
}

} // namespace bsgo
