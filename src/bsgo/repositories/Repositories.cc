
#include "Repositories.hh"

namespace bsgo {

Repositories::Repositories()
{
  dbConnection = std::make_shared<DbConnection>();
  dbConnection->connect();

  accountRepository        = std::make_shared<AccountRepository>(dbConnection);
  aiBehaviorRepository     = std::make_shared<AiBehaviorRepository>(dbConnection);
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
  shipComputerRepository   = std::make_shared<ShipComputerRepository>(dbConnection);
  shipPriceRepository      = std::make_shared<ShipPriceRepository>(dbConnection);
  shipRepository           = std::make_shared<ShipRepository>(dbConnection);
  shipWeaponRepository     = std::make_shared<ShipWeaponRepository>(dbConnection);
  systemOutpostRepository  = std::make_shared<SystemOutpostRepository>(dbConnection);
  systemRepository         = std::make_shared<SystemRepository>(dbConnection);
  weaponPriceRepository    = std::make_shared<WeaponPriceRepository>(dbConnection);
  weaponRepository         = std::make_shared<WeaponRepository>(dbConnection);
  tickRepository           = std::make_shared<TickRepository>(dbConnection);

  accountRepository->initialize();
  aiBehaviorRepository->initialize();
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
  shipComputerRepository->initialize();
  shipPriceRepository->initialize();
  shipRepository->initialize();
  shipWeaponRepository->initialize();
  systemOutpostRepository->initialize();
  systemRepository->initialize();
  weaponPriceRepository->initialize();
  weaponRepository->initialize();
  tickRepository->initialize();
}

} // namespace bsgo
