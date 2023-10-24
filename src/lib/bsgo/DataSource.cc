
#include "DataSource.hh"
#include "CircleBox.hh"

namespace bsgo {

DataSource::DataSource()
  : utils::CoreObject("bsgo")
{
  setService("data");
}

auto DataSource::playerId() const -> Uuid
{
  return m_playerId;
}

auto DataSource::playerShipId() const -> Uuid
{
  return m_playerRepo.findShipById(m_playerId);
}

void DataSource::initialize(Coordinator &coordinator) const
{
  const auto systemId = m_playerRepo.findSystemById(m_playerId);
  initializeShips(coordinator, systemId);
  initializeAsteroids(coordinator, systemId);
  initializeOutposts(coordinator, systemId);
}

void DataSource::initializeAsteroids(Coordinator &coordinator, const Uuid &system) const
{
  const auto asteroids = m_systemRepo.findAllAsteroidsBySystem(system);
  for (const auto &id : asteroids)
  {
    registerAsteroid(coordinator, id);
  }
}

void DataSource::initializeShips(Coordinator &coordinator, const Uuid &system) const
{
  const auto ships = m_systemRepo.findAllShipsBySystem(system);
  for (const auto &id : ships)
  {
    registerShip(coordinator, id);
  }
}

void DataSource::initializeOutposts(Coordinator &coordinator, const Uuid &system) const
{
  const auto outposts = m_systemRepo.findAllOutpostsBySystem(system);
  for (const auto &id : outposts)
  {
    registerOutpost(coordinator, id);
  }
}

void DataSource::registerAsteroid(Coordinator &coordinator, const Uuid &asteroid) const
{
  const auto data = m_asteroidRepo.findOneById(asteroid);

  auto box       = std::make_unique<CircleBox>(data.position, data.radius);
  const auto ent = coordinator.createEntity(EntityKind::ASTEROID);
  coordinator.addTransform(ent, std::move(box));
  coordinator.addHealth(ent, data.health, data.health, 0.0f);
  coordinator.addScanned(ent);
  if (data.resource)
  {
    coordinator.addLoot(ent, *data.resource);
  }
}

void DataSource::registerShip(Coordinator &coordinator, const Uuid &ship) const
{
  const auto data = m_playerShipRepo.findOneById(ship);

  auto box       = std::make_unique<CircleBox>(data.position, data.radius);
  const auto ent = coordinator.createEntity(EntityKind::SHIP);
  coordinator.addTransform(ent, std::move(box));
  coordinator.addVelocity(ent, data.acceleration);
  coordinator.addHealth(ent, data.hullPoints, data.maxHullPoints, data.hullPointsRegen);
  coordinator.addPower(ent, data.powerPoints, data.maxPowerPoints, data.powerRegen);
  coordinator.addTarget(ent);
  coordinator.addFaction(ent, data.faction);

  for (const auto &weapon : data.weapons)
  {
    registerWeapon(coordinator, ent, weapon);
  }
  for (const auto &computer : data.computers)
  {
    registerComputer(coordinator, ent, computer);
  }
}

void DataSource::registerOutpost(Coordinator &coordinator, const Uuid &outpost) const
{
  const auto data = m_outpostRepo.findOneById(outpost);

  auto box       = std::make_unique<CircleBox>(data.position, data.radius);
  const auto ent = coordinator.createEntity(EntityKind::OUTPOST);
  coordinator.addTransform(ent, std::move(box));
  coordinator.addHealth(ent, data.hullPoints, data.maxHullPoints, data.hullPointsRegen);
  coordinator.addPower(ent, data.powerPoints, data.maxPowerPoints, data.powerRegen);
  coordinator.addTarget(ent);
  coordinator.addFaction(ent, data.faction);
}

void DataSource::registerWeapon(Coordinator &coordinator, const Uuid &ship, const Uuid &weapon) const
{
  const auto data = m_weaponRepository.findOneById(weapon);
  coordinator.addWeapon(ship, data);
}

void DataSource::registerComputer(Coordinator &coordinator,
                                  const Uuid &ship,
                                  const Uuid &computer) const
{
  const auto data = m_computerRepository.findOneById(computer);
  coordinator.addComputer(ship, data);
}

} // namespace bsgo
