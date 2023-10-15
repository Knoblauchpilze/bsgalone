
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
    const auto asteroid = m_asteroidRepo.findOneById(id);

    auto box       = std::make_unique<CircleBox>(asteroid.position, asteroid.radius);
    const auto ent = coordinator.createEntity(EntityKind::ASTEROID);
    coordinator.addTransform(ent, std::move(box));
    coordinator.addHealth(ent, asteroid.health, asteroid.health, 0.0f);
  }
}

void DataSource::initializeShips(Coordinator &coordinator, const Uuid &system) const
{
  const auto ships = m_systemRepo.findAllShipsBySystem(system);
  for (const auto &id : ships)
  {
    const auto ship = m_playerShipRepo.findOneById(id);

    auto box       = std::make_unique<CircleBox>(ship.position, ship.radius);
    const auto ent = coordinator.createEntity(EntityKind::SHIP);
    coordinator.addTransform(ent, std::move(box));
    coordinator.addVelocity(ent, ship.acceleration);
    coordinator.addHealth(ent, ship.hullPoints, ship.maxHullPoints, ship.hullPointsRegen);
    coordinator.addPower(ent, ship.powerPoints, ship.maxPowerPoints, ship.powerRegen);
  }
}

void DataSource::initializeOutposts(Coordinator &coordinator, const Uuid &system) const
{
  const auto outposts = m_systemRepo.findAllOutpostsBySystem(system);
  for (const auto &id : outposts)
  {
    const auto outpost = m_outpostRepo.findOneById(id);

    auto box       = std::make_unique<CircleBox>(outpost.position, outpost.radius);
    const auto ent = coordinator.createEntity(EntityKind::OUTPOST);
    coordinator.addTransform(ent, std::move(box));
    coordinator.addHealth(ent, outpost.hullPoints, outpost.maxHullPoints, outpost.hullPointsRegen);
    coordinator.addPower(ent, outpost.powerPoints, outpost.maxPowerPoints, outpost.powerRegen);
  }
}

} // namespace bsgo
