
#include "LoadingService.hh"

namespace bsgo {

LoadingService::LoadingService(const Repositories &repositories,
                               CoordinatorShPtr coordinator,
                               const DatabaseEntityMapper &entityMapper)
  : AbstractService("loading", repositories)
  , m_coordinator(std::move(coordinator))
  , m_entityMapper(entityMapper)
{}

auto LoadingService::getDataForPlayer(const Uuid playerDbId) const -> PlayerProps
{
  const auto player = m_repositories.playerRepository->findOneById(playerDbId);
  const auto ship   = m_repositories.playerShipRepository->findOneByPlayerAndActive(playerDbId);

  if (!ship.system)
  {
    error("Failed to get data for player " + str(playerDbId),
          "Active ship does not have a system assigned");
  }

  return {
    .faction    = player.faction,
    .shipDbId   = ship.id,
    .docked     = ship.docked,
    .systemDbId = *ship.system,
  };
}

auto LoadingService::getPlayerById(const Uuid playerDbId) const -> Player
{
  return m_repositories.playerRepository->findOneById(playerDbId);
}

auto LoadingService::getPlayersInSystem(const Uuid systemDbId) const -> std::vector<Player>
{
  const auto playerIds = m_repositories.playerRepository->findAllUndockedBySystem(systemDbId);

  std::vector<Player> players{};

  for (const auto &playerId : playerIds)
  {
    const auto player = m_repositories.playerRepository->findOneById(playerId);
    players.emplace_back(player);
  }

  return players;
}

auto LoadingService::getAsteroidById(const Uuid asteroidDbId) const -> AsteroidProps
{
  const auto asteroid = m_repositories.asteroidRepository->findOneById(asteroidDbId);

  std::optional<Uuid> resource{};
  std::optional<float> amount{};
  if (asteroid.loot)
  {
    const auto loot = m_repositories.asteroidLootRepository->findOneById(asteroidDbId);
    resource        = loot.resource;
    amount          = loot.amount;
  }

  return AsteroidProps{
    .dbId       = asteroidDbId,
    .dbAsteroid = asteroid,
    .resource   = resource,
    .amount     = amount,
  };
}

auto LoadingService::getAsteroidsInSystem(const Uuid systemDbId) const -> std::vector<AsteroidProps>
{
  const auto asteroidDbIds = m_repositories.systemRepository->findAllAsteroidsBySystem(systemDbId);

  std::vector<AsteroidProps> asteroids{};

  for (const auto &asteroidDbId : asteroidDbIds)
  {
    const auto asteroid = m_repositories.asteroidRepository->findOneById(asteroidDbId);

    std::optional<Uuid> resource{};
    std::optional<float> amount{};
    if (asteroid.loot)
    {
      const auto loot = m_repositories.asteroidLootRepository->findOneById(asteroidDbId);
      resource        = loot.resource;
      amount          = loot.amount;
    }

    asteroids.emplace_back(asteroidDbId, asteroid, resource, amount);
  }

  return asteroids;
}

namespace {
auto getOutpostTargetDbId(const Uuid outpostDbId,
                          const DatabaseEntityMapper &entityMapper,
                          const Coordinator &coordinator) -> std::optional<Uuid>
{
  const auto maybeEntityId = entityMapper.tryGetOutpostEntityId(outpostDbId);
  if (!maybeEntityId)
  {
    return {};
  }

  const auto entity = coordinator.getEntity(*maybeEntityId);
  return entity.targetComp().target();
}
} // namespace

auto LoadingService::getOutpostById(const Uuid outpostDbId) const -> OutpostProps
{
  const auto outpost    = m_repositories.systemOutpostRepository->findOneById(outpostDbId);
  const auto targetDbId = getOutpostTargetDbId(outpostDbId, m_entityMapper, *m_coordinator);

  return OutpostProps{
    .dbId       = outpostDbId,
    .dbOutpost  = outpost,
    .targetDbId = targetDbId,
  };
}

auto LoadingService::getOutpostsInSystem(const Uuid systemDbId) const -> std::vector<OutpostProps>
{
  const auto outpostDbIds = m_repositories.systemRepository->findAllOutpostsBySystem(systemDbId);

  std::vector<OutpostProps> outposts{};

  for (const auto &outpostDbId : outpostDbIds)
  {
    const auto outpost = m_repositories.systemOutpostRepository->findOneById(outpostDbId);

    const auto targetDbId = getOutpostTargetDbId(outpostDbId, m_entityMapper, *m_coordinator);
    outposts.emplace_back(outpostDbId, outpost, targetDbId);
  }

  return outposts;
}

namespace {
auto getWeaponsForShip(const Repositories &repositories, const Uuid shipDbId)
  -> std::vector<WeaponProps>
{
  std::vector<WeaponProps> weapons{};

  const auto shipWeapons = repositories.shipWeaponRepository->findAllByShip(shipDbId);
  for (const auto &weapon : shipWeapons)
  {
    const auto data = repositories.playerWeaponRepository->findOneById(weapon.weapon);

    weapons.emplace_back(data, weapon.slotPosition);
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

auto getShipTargetDbId(const Uuid shipDbId,
                       const DatabaseEntityMapper &entityMapper,
                       const Coordinator &coordinator) -> std::optional<Uuid>
{
  const auto maybeEntityId = entityMapper.tryGetShipEntityId(shipDbId);
  if (!maybeEntityId)
  {
    return {};
  }

  const auto entity = coordinator.getEntity(*maybeEntityId);
  return entity.targetComp().target();
}
} // namespace

auto LoadingService::getShipById(const Uuid shipDbId) const -> ShipProps
{
  const auto ship       = m_repositories.playerShipRepository->findOneById(shipDbId);
  const auto weapons    = getWeaponsForShip(m_repositories, shipDbId);
  const auto computers  = getComputersForShip(m_repositories, shipDbId);
  const auto status     = determineStartingStatusForShip(ship);
  const auto targetDbId = getShipTargetDbId(shipDbId, m_entityMapper, *m_coordinator);

  return ShipProps{
    .dbShip     = ship,
    .status     = status,
    .targetDbId = targetDbId,
    .weapons    = weapons,
    .computers  = computers,
  };
}

auto LoadingService::getShipsInSystem(const Uuid systemDbId) const -> std::vector<ShipProps>
{
  const auto shipDbIds = m_repositories.systemRepository->findAllShipsBySystem(systemDbId);

  std::vector<ShipProps> ships{};

  for (const auto &shipDbId : shipDbIds)
  {
    const auto ship = m_repositories.playerShipRepository->findOneById(shipDbId);

    const auto weapons   = getWeaponsForShip(m_repositories, shipDbId);
    const auto computers = getComputersForShip(m_repositories, shipDbId);

    const auto status     = determineStartingStatusForShip(ship);
    const auto targetDbId = getShipTargetDbId(shipDbId, m_entityMapper, *m_coordinator);

    ships.emplace_back(ship, status, targetDbId, weapons, computers);
  }

  return ships;
}

auto LoadingService::getSystems() const -> std::vector<System>
{
  std::vector<System> out;

  const auto ids = m_repositories.systemRepository->findAll();
  for (const auto &id : ids)
  {
    auto system = m_repositories.systemRepository->findOneById(id);
    out.push_back(std::move(system));
  }

  return out;
}

} // namespace bsgo
