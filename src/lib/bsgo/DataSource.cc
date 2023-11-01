
#include "DataSource.hh"
#include "CircleBox.hh"
#include "Coordinator.hh"

namespace bsgo {

DataSource::DataSource()
  : utils::CoreObject("bsgo")
{
  setService("data");
}

auto DataSource::repositories() const -> Repositories
{
  return m_repositories;
}

auto DataSource::playerId() const -> Uuid
{
  return m_playerId;
}

auto DataSource::playerEntityId() const -> Uuid
{
  if (!m_playerEntityId)
  {
    error("Expected to have a player entity id");
  }
  return *m_playerEntityId;
}

auto DataSource::playerShipEntityId() const -> Uuid
{
  if (!m_playerShipEntityId)
  {
    error("Expected to have a player ship entity id");
  }
  return *m_playerShipEntityId;
}

void DataSource::initialize(Coordinator &coordinator) const
{
  const auto systemId = m_repositories.playerRepository->findSystemById(m_playerId);

  initializePlayer(coordinator);
  initializeShips(coordinator, systemId);
  initializeAsteroids(coordinator, systemId);
  initializeOutposts(coordinator, systemId);
}

void DataSource::initializePlayer(Coordinator &coordinator) const
{
  m_playerEntityId = coordinator.createEntity(EntityKind::PLAYER);

  const auto resources = m_repositories.playerResourceRepository->findAllByPlayer(m_playerId);
  for (const auto &resource : resources)
  {
    coordinator.addResourceComponent(*m_playerEntityId, resource.resource, resource.amount);
  }
}

void DataSource::initializeAsteroids(Coordinator &coordinator, const Uuid &system) const
{
  const auto asteroids = m_repositories.systemRepository->findAllAsteroidsBySystem(system);
  for (const auto &id : asteroids)
  {
    registerAsteroid(coordinator, id);
  }
}

void DataSource::initializeShips(Coordinator &coordinator, const Uuid &system) const
{
  const auto ships = m_repositories.systemRepository->findAllShipsBySystem(system);
  for (const auto &id : ships)
  {
    registerShip(coordinator, id);
  }
}

void DataSource::initializeOutposts(Coordinator &coordinator, const Uuid &system) const
{
  const auto outposts = m_repositories.systemRepository->findAllOutpostsBySystem(system);
  for (const auto &id : outposts)
  {
    registerOutpost(coordinator, id);
  }
}

void DataSource::registerAsteroid(Coordinator &coordinator, const Uuid &asteroid) const
{
  const auto data = m_repositories.asteroidRepository->findOneById(asteroid);

  auto box       = std::make_unique<CircleBox>(data.position, data.radius);
  const auto ent = coordinator.createEntity(EntityKind::ASTEROID);
  coordinator.addTransform(ent, std::move(box));
  coordinator.addHealth(ent, data.health, data.health, 0.0f);
  coordinator.addScanned(ent);
  if (data.loot)
  {
    coordinator.addLoot(ent);
    const auto loot = m_repositories.asteroidLootRepository->findOneById(asteroid);
    coordinator.addResourceComponent(ent, loot.resource, loot.amount);
  }
}

void DataSource::registerShip(Coordinator &coordinator, const Uuid &ship) const
{
  const auto data = m_repositories.playerShipRepository->findOneById(ship);

  auto box       = std::make_unique<CircleBox>(data.position, data.radius);
  const auto ent = coordinator.createEntity(EntityKind::SHIP);
  coordinator.addTransform(ent, std::move(box));
  VelocityData vData{.maxAcceleration = data.acceleration, .maxSpeed = data.speed};
  coordinator.addVelocity(ent, vData);
  coordinator.addHealth(ent, data.hullPoints, data.maxHullPoints, data.hullPointsRegen);
  coordinator.addPower(ent, data.powerPoints, data.maxPowerPoints, data.powerRegen);
  coordinator.addTarget(ent);
  coordinator.addFaction(ent, data.faction);
  if (data.player && *data.player == m_playerId)
  {
    if (!m_playerEntityId)
    {
      error("Failed to create ship " + str(ship), "Expected player entity to be created already");
    }
    if (m_playerShipEntityId)
    {
      error("Failed to create ship " + str(ship),
            "Player ship id is already assigned entity " + str(*m_playerShipEntityId));
    }

    m_playerShipEntityId = ent;
    coordinator.addOwner(ent, *m_playerEntityId);
  }

  registerShipWeapons(coordinator, ship, ent);
  registerShipComputers(coordinator, ship, ent);
}

void DataSource::registerOutpost(Coordinator &coordinator, const Uuid &outpost) const
{
  const auto data = m_repositories.outpostRepository->findOneById(outpost);

  auto box       = std::make_unique<CircleBox>(data.position, data.radius);
  const auto ent = coordinator.createEntity(EntityKind::OUTPOST);
  coordinator.addTransform(ent, std::move(box));
  coordinator.addHealth(ent, data.hullPoints, data.maxHullPoints, data.hullPointsRegen);
  coordinator.addPower(ent, data.powerPoints, data.maxPowerPoints, data.powerRegen);
  coordinator.addTarget(ent);
  coordinator.addFaction(ent, data.faction);
}

void DataSource::registerShipWeapons(Coordinator &coordinator,
                                     const Uuid &ship,
                                     const Uuid &shipEntity) const
{
  const auto weapons = m_repositories.shipWeaponRepository->findAllById(ship);
  for (const auto &weapon : weapons)
  {
    const auto data = m_repositories.weaponRepository->findOneById(weapon);
    coordinator.addWeapon(shipEntity, data);
  }
}

void DataSource::registerShipComputers(Coordinator &coordinator,
                                       const Uuid &ship,
                                       const Uuid &shipEntity) const
{
  const auto computers = m_repositories.shipComputerRepository->findAllById(ship);
  for (const auto &computer : computers)
  {
    const auto data = m_repositories.computerRepository->findOneById(computer);
    coordinator.addComputer(shipEntity, data);
  }
}

} // namespace bsgo
