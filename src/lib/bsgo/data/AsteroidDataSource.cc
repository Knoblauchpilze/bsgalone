
#include "AsteroidDataSource.hh"
#include "CircleBox.hh"
#include "Coordinator.hh"

namespace bsgo {

AsteroidDataSource::AsteroidDataSource(const Repositories &repositories, const Uuid &playerDbId)
  : utils::CoreObject("bsgo")
  , m_playerDbId(playerDbId)
  , m_repositories(repositories)
{
  setService("data");
  addModule("asteroid");
}

void AsteroidDataSource::initialize(Coordinator &coordinator) const
{
  const auto systemId = m_repositories.playerRepository->findSystemByPlayer(m_playerDbId);

  const auto asteroids = m_repositories.systemRepository->findAllAsteroidsBySystem(systemId);
  for (const auto &id : asteroids)
  {
    registerAsteroid(coordinator, id);
  }
}

void AsteroidDataSource::registerAsteroid(Coordinator &coordinator, const Uuid &asteroid) const
{
  const auto data = m_repositories.asteroidRepository->findOneById(asteroid);

  auto box       = std::make_unique<CircleBox>(data.position, data.radius);
  const auto ent = coordinator.createEntity(EntityKind::ASTEROID);
  coordinator.addTransform(ent, std::move(box));
  coordinator.addHealth(ent, data.health, data.health, 0.0f);
  coordinator.addRemoval(ent);
  coordinator.addScanned(ent);
  if (data.loot)
  {
    coordinator.addLoot(ent);
    const auto loot = m_repositories.asteroidLootRepository->findOneById(asteroid);
    coordinator.addResourceComponent(ent, loot.resource, loot.amount);
  }
}

} // namespace bsgo
