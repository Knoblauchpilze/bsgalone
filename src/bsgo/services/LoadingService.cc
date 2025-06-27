
#include "LoadingService.hh"

namespace bsgo {

LoadingService::LoadingService(const Repositories &repositories,
                               CoordinatorShPtr coordinator,
                               const DatabaseEntityMapper &entityMapper)
  : AbstractService("loading", repositories)
  , m_coordinator(std::move(coordinator))
  , m_entityMapper(entityMapper)
{}

auto LoadingService::getPlayersInSystem(const Uuid systemDbId) const -> std::vector<Player>
{
  const auto playerIds = m_repositories.playerRepository->findAllBySystem(systemDbId);

  std::vector<Player> players{};

  for (const auto &playerId : playerIds)
  {
    const auto player = m_repositories.playerRepository->findOneById(playerId);
    players.emplace_back(player);
  }

  return players;
}

auto LoadingService::getAsteroidsInSystem(const Uuid systemDbId) const -> std::vector<AsteroidProps>
{
  const auto asteroidIds = m_repositories.systemRepository->findAllAsteroidsBySystem(systemDbId);

  std::vector<AsteroidProps> asteroids{};

  for (const auto &asteroidId : asteroidIds)
  {
    const auto asteroid = m_repositories.asteroidRepository->findOneById(asteroidId);

    std::optional<Uuid> resource{};
    std::optional<float> amount{};
    if (asteroid.loot)
    {
      const auto loot = m_repositories.asteroidLootRepository->findOneById(asteroidId);
      resource        = loot.resource;
      amount          = loot.amount;
    }

    asteroids.emplace_back(asteroidId, asteroid, resource, amount);
  }

  return asteroids;
}

auto LoadingService::getOutpostsInSystem(const Uuid systemDbId) const -> std::vector<OutpostProps>
{
  const auto outpostIds = m_repositories.systemRepository->findAllOutpostsBySystem(systemDbId);

  std::vector<OutpostProps> outposts{};

  for (const auto &outpostId : outpostIds)
  {
    const auto outpost = m_repositories.systemOutpostRepository->findOneById(outpostId);

    // TODO: the target id is left empty for now. We should give this service
    // access to the coordinator and the entity mapper and try to fetch it
    // from there. Probably in a dedicated method.
    outposts.emplace_back(outpostId, outpost);
  }

  return outposts;
}

namespace {
auto getWeaponsForShip(const Repositories &repositories, const Uuid shipDbId)
  -> std::vector<PlayerWeapon>
{
  std::vector<PlayerWeapon> weapons{};

  const auto shipWeapons = repositories.shipWeaponRepository->findAllByShip(shipDbId);
  for (const auto &weapon : shipWeapons)
  {
    const auto data = repositories.playerWeaponRepository->findOneById(weapon.weapon);

    // TODO: Handle slot position
    weapons.push_back(data);
  }

  return weapons;
}

auto getComputersForShip(const Repositories &repositories, const Uuid shipDbId)
  -> std::vector<PlayerComputer>
{
  std::vector<PlayerComputer> computers{};

  const auto shipComputers = repositories.shipComputerRepository->findAllByShip(shipDbId);
  for (const auto &computer : shipComputers)
  {
    const auto data = repositories.playerComputerRepository->findOneById(computer);
    computers.push_back(data);
  }

  return computers;
}
} // namespace

auto LoadingService::getShipsInSystem(const Uuid systemDbId) const -> std::vector<ShipProps>
{
  const auto shipIds = m_repositories.systemRepository->findAllShipsBySystem(systemDbId);

  std::vector<ShipProps> ships{};

  for (const auto &shipId : shipIds)
  {
    const auto ship = m_repositories.playerShipRepository->findOneById(shipId);

    const auto weapons   = getWeaponsForShip(m_repositories, shipId);
    const auto computers = getComputersForShip(m_repositories, shipId);

    // TODO: the target id is left empty for now. We should
    // give this service access to the coordinator and the
    // entity mapper and try to fetch it from there.
    const auto status = determineStartingStatusForShip(ship);
    std::optional<Uuid> targetDbId{};

    ships.emplace_back(ship, status, targetDbId, weapons, computers);
  }

  return ships;
}

} // namespace bsgo
