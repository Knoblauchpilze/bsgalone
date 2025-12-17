
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
  const auto playerIds = m_repositories.playerRepository->findAllBySystem(systemDbId);

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
  std::optional<int> amount{};
  if (asteroid.loot)
  {
    const auto loot = m_repositories.asteroidLootRepository->findOneById(asteroidDbId);
    resource        = loot.resource;
    amount          = loot.amount;
  }

  return AsteroidProps{
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
    std::optional<int> amount{};
    if (asteroid.loot)
    {
      const auto loot = m_repositories.asteroidLootRepository->findOneById(asteroidDbId);
      resource        = loot.resource;
      amount          = loot.amount;
    }

    asteroids.emplace_back(asteroid, resource, amount);
  }

  return asteroids;
}

auto LoadingService::getOutpostById(const Uuid outpostDbId) const -> OutpostProps
{
  const auto outpost = m_repositories.systemOutpostRepository->findOneById(outpostDbId);

  return OutpostProps{
    .dbOutpost = outpost,
  };
}

auto LoadingService::getOutpostsInSystem(const Uuid systemDbId) const -> std::vector<OutpostProps>
{
  const auto outpostDbIds = m_repositories.systemRepository->findAllOutpostsBySystem(systemDbId);

  std::vector<OutpostProps> outposts{};

  for (const auto &outpostDbId : outpostDbIds)
  {
    const auto outpost = m_repositories.systemOutpostRepository->findOneById(outpostDbId);

    outposts.emplace_back(outpost);
  }

  return outposts;
}

namespace {
auto getWeaponsForShip(const Repositories &repositories, const Uuid shipDbId)
  -> std::vector<PlayerWeaponProps>
{
  std::vector<PlayerWeaponProps> weapons{};

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
} // namespace

auto LoadingService::getShipById(const Uuid shipDbId) const -> PlayerShipProps
{
  const auto ship      = m_repositories.playerShipRepository->findOneById(shipDbId);
  const auto player    = m_repositories.playerRepository->findOneById(ship.player);
  const auto weapons   = getWeaponsForShip(m_repositories, shipDbId);
  const auto computers = getComputersForShip(m_repositories, shipDbId);
  const auto status    = determineStartingStatusForShip(ship, player);

  return PlayerShipProps{
    .dbShip    = ship,
    .status    = status,
    .weapons   = weapons,
    .computers = computers,
  };
}

auto LoadingService::getShipsInSystem(const Uuid systemDbId) const -> std::vector<PlayerShipProps>
{
  const auto shipDbIds = m_repositories.systemRepository->findAllShipsBySystem(systemDbId);

  std::vector<PlayerShipProps> ships{};

  for (const auto &shipDbId : shipDbIds)
  {
    const auto ship   = m_repositories.playerShipRepository->findOneById(shipDbId);
    const auto player = m_repositories.playerRepository->findOneById(ship.player);

    const auto weapons   = getWeaponsForShip(m_repositories, shipDbId);
    const auto computers = getComputersForShip(m_repositories, shipDbId);

    const auto status = determineStartingStatusForShip(ship, player);

    ships.emplace_back(ship, status, weapons, computers);
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

auto LoadingService::getSystemTickConfig(const Uuid systemDbId) const -> SystemTick
{
  return m_repositories.tickRepository->findOneBySystem(systemDbId);
}

namespace {
auto fetchSystemOutpostTarget(const Uuid outpostDbId,
                              const DatabaseEntityMapper &entityMapper,
                              const Coordinator &coordinator) -> std::optional<TargetProps>
{
  const auto maybeEntityId = entityMapper.tryGetOutpostEntityId(outpostDbId);
  if (!maybeEntityId)
  {
    return {};
  }

  const auto outpostEntity = coordinator.getEntity(*maybeEntityId);
  const auto &targetComp   = outpostEntity.targetComp();

  if (!targetComp.target())
  {
    return {};
  }

  const auto targetEntity = coordinator.getEntity(*targetComp.target());

  return TargetProps{
    .sourceDbId = outpostDbId,
    .sourceKind = EntityKind::OUTPOST,
    .targetDbId = targetEntity.dbComp().dbId(),
    .targetKind = targetEntity.kind->kind(),
  };
}

auto fetchSystemOutpostsTargets(const Repositories &repositories,
                                const Uuid systemDbId,
                                const DatabaseEntityMapper &entityMapper,
                                const Coordinator &coordinator) -> std::vector<TargetProps>
{
  std::vector<TargetProps> out{};

  const auto outpostsDbIds = repositories.systemRepository->findAllOutpostsBySystem(systemDbId);

  for (const auto &outpostDbId : outpostsDbIds)
  {
    const auto maybeTarget = fetchSystemOutpostTarget(outpostDbId, entityMapper, coordinator);
    if (maybeTarget)
    {
      out.push_back(*maybeTarget);
    }
  }

  return out;
}

auto fetchPlayerShipTarget(const Uuid shipDbId,
                           const DatabaseEntityMapper &entityMapper,
                           const Coordinator &coordinator) -> std::optional<TargetProps>
{
  const auto maybeEntityId = entityMapper.tryGetShipEntityId(shipDbId);
  if (!maybeEntityId)
  {
    return {};
  }

  const auto shipEntity  = coordinator.getEntity(*maybeEntityId);
  const auto &targetComp = shipEntity.targetComp();

  if (!targetComp.target())
  {
    return {};
  }

  const auto targetEntity = coordinator.getEntity(*targetComp.target());

  return TargetProps{
    .sourceDbId = shipDbId,
    .sourceKind = EntityKind::SHIP,
    .targetDbId = targetEntity.dbComp().dbId(),
    .targetKind = targetEntity.kind->kind(),
  };
}

auto fetchPlayerShipsTargets(const Repositories &repositories,
                             const Uuid systemDbId,
                             const DatabaseEntityMapper &entityMapper,
                             const Coordinator &coordinator) -> std::vector<TargetProps>
{
  std::vector<TargetProps> out{};

  const auto shipsDbIds = repositories.systemRepository->findAllShipsBySystem(systemDbId);

  for (const auto &shipDbId : shipsDbIds)
  {
    const auto maybeTarget = fetchPlayerShipTarget(shipDbId, entityMapper, coordinator);
    if (maybeTarget)
    {
      out.push_back(*maybeTarget);
    }
  }

  return out;
}
} // namespace

auto LoadingService::getTargetsInSystem(const Uuid systemDbId) const -> std::vector<TargetProps>
{
  std::vector<TargetProps> out{};

  const auto outpostsTargets = fetchSystemOutpostsTargets(m_repositories,
                                                          systemDbId,
                                                          m_entityMapper,
                                                          *m_coordinator);
  out.insert(out.end(), outpostsTargets.begin(), outpostsTargets.end());

  const auto playerShipsTargets = fetchPlayerShipsTargets(m_repositories,
                                                          systemDbId,
                                                          m_entityMapper,
                                                          *m_coordinator);
  out.insert(out.end(), playerShipsTargets.begin(), playerShipsTargets.end());

  return out;
}

auto LoadingService::getResources() const -> std::vector<Resource>
{
  std::vector<Resource> out;

  const auto ids = m_repositories.resourceRepository->findAll();
  for (const auto &id : ids)
  {
    auto resource = m_repositories.resourceRepository->findOneById(id);
    out.push_back(std::move(resource));
  }

  return out;
}

auto LoadingService::getWeapons() const -> std::vector<WeaponProps>
{
  std::vector<WeaponProps> out;

  for (const auto &id : m_repositories.weaponRepository->findAll())
  {
    auto weapon      = m_repositories.weaponRepository->findOneById(id);
    const auto price = m_repositories.weaponPriceRepository->findAllByWeapon(id);

    out.emplace_back(std::move(weapon), price);
  }

  return out;
}

auto LoadingService::getComputers() const -> std::vector<ComputerProps>
{
  std::vector<ComputerProps> out;

  for (const auto &id : m_repositories.computerRepository->findAll())
  {
    auto computer    = m_repositories.computerRepository->findOneById(id);
    const auto price = m_repositories.computerPriceRepository->findAllByComputer(id);

    out.emplace_back(std::move(computer), price);
  }

  return out;
}

auto LoadingService::getShipsForFaction(const Faction faction) const -> std::vector<ShipProps>
{
  std::vector<ShipProps> out;

  for (const auto &ship : m_repositories.shipRepository->findAllByFaction(faction))
  {
    const auto price = m_repositories.shipPriceRepository->findAllByShip(ship.id);

    out.emplace_back(ship, price);
  }

  return out;
}

auto LoadingService::getPlayerResources(const Uuid playerDbId) const -> std::vector<PlayerResource>
{
  return m_repositories.playerResourceRepository->findAllByPlayer(playerDbId);
}

auto LoadingService::getPlayerShips(const Uuid playerDbId) const -> std::vector<PlayerShipProps>
{
  const auto shipDbIds = m_repositories.playerShipRepository->findAllByPlayer(playerDbId);

  std::vector<PlayerShipProps> ships{};

  for (const auto &shipDbId : shipDbIds)
  {
    const auto ship   = m_repositories.playerShipRepository->findOneById(shipDbId);
    const auto player = m_repositories.playerRepository->findOneById(ship.player);

    const auto weapons   = getWeaponsForShip(m_repositories, shipDbId);
    const auto computers = getComputersForShip(m_repositories, shipDbId);

    const auto status = determineStartingStatusForShip(ship, player);

    ships.emplace_back(ship, status, weapons, computers);
  }

  return ships;
}

auto LoadingService::getPlayerComputers(const Uuid playerDbId) const -> std::vector<PlayerComputer>
{
  const auto computerDbIds = m_repositories.playerComputerRepository->findAllByPlayer(playerDbId);

  std::vector<PlayerComputer> computers{};

  for (const auto &computerDbId : computerDbIds)
  {
    const auto computer = m_repositories.playerComputerRepository->findOneById(computerDbId);

    computers.emplace_back(computer);
  }

  return computers;
}

auto LoadingService::getPlayerWeapons(const Uuid playerDbId) const -> std::vector<PlayerWeaponProps>
{
  const auto weaponDbIds = m_repositories.playerWeaponRepository->findAllByPlayer(playerDbId);

  std::vector<PlayerWeaponProps> weapons{};

  for (const auto &weaponDbId : weaponDbIds)
  {
    const auto weapon = m_repositories.playerWeaponRepository->findOneById(weaponDbId);

    weapons.emplace_back(weapon, std::optional<Eigen::Vector3f>{});
  }

  return weapons;
}

auto LoadingService::getActivePlayerShip(const Uuid playerDbId) const -> PlayerShipProps
{
  const auto ship   = m_repositories.playerShipRepository->findOneByPlayerAndActive(playerDbId);
  const auto player = m_repositories.playerRepository->findOneById(ship.player);

  const auto weapons   = getWeaponsForShip(m_repositories, ship.id);
  const auto computers = getComputersForShip(m_repositories, ship.id);

  const auto status = determineStartingStatusForShip(ship, player);

  return PlayerShipProps{
    .dbShip    = ship,
    .status    = status,
    .weapons   = weapons,
    .computers = computers,
  };
}

} // namespace bsgo
