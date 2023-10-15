
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
  initializeAsteroids(coordinator, systemId);
  initializeShips(coordinator, systemId);
  initializeOutposts(coordinator, systemId);
}

void DataSource::initializeAsteroids(Coordinator &coordinator, const Uuid &system) const
{
  const auto ships = m_systemRepo.findAllShipsBySystem(system);
  for (const auto &id : ships)
  {
    const auto ship = m_playerShipRepo.findOneById(id);

    auto box       = std::make_unique<CircleBox>(ship.position, ship.radius);
    const auto ent = coordinator.createEntity(EntityKind::SHIP);
    coordinator.addTransform(ent, std::move(box));
    coordinator.addVelocity(ent, Eigen::Vector3f::Zero());
    coordinator.addHealth(ent, ship.hullPoints, ship.maxHullPoints);
    coordinator.addPower(ent, ship.powerPoints, ship.maxPowerPoints);
  }
}

void DataSource::initializeShips(Coordinator &coordinator, const Uuid &system) const
{
  const auto asteroids = m_systemRepo.findAllAsteroidsBySystem(system);
  for (const auto &id : asteroids)
  {
    const auto asteroid = m_asteroidRepo.findOneById(id);

    auto box       = std::make_unique<CircleBox>(asteroid.position, asteroid.radius);
    const auto ent = coordinator.createEntity(EntityKind::ASTEROID);
    coordinator.addTransform(ent, std::move(box));
    coordinator.addHealth(ent, asteroid.health, asteroid.health);
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
    log("entity outpost is " + std::to_string(ent) + " with radius "
        + std::to_string(outpost.radius));
    coordinator.addTransform(ent, std::move(box));
    coordinator.addHealth(ent, outpost.hullPoints, outpost.hullPoints);
    coordinator.addPower(ent, outpost.powerPoints, outpost.powerPoints);
  }
}

} // namespace bsgo
