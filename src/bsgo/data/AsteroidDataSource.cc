
#include "AsteroidDataSource.hh"
#include "CircleBox.hh"
#include "Coordinator.hh"

namespace bsgo {

AsteroidDataSource::AsteroidDataSource(const Repositories &repositories, const Uuid systemDbId)
  : utils::CoreObject("bsgo")
  , m_systemDbId(systemDbId)
  , m_repositories(repositories)
{
  setService("data");
  addModule("asteroid");
}

void AsteroidDataSource::initialize(Coordinator &coordinator) const
{
  const auto asteroids = m_repositories.systemRepository->findAllAsteroidsBySystem(m_systemDbId);
  for (const auto &id : asteroids)
  {
    registerAsteroid(coordinator, id);
  }
}

void AsteroidDataSource::registerAsteroid(Coordinator &coordinator, const Uuid asteroid) const
{
  const auto data = m_repositories.asteroidRepository->findOneById(asteroid);

  auto box       = std::make_unique<CircleBox>(data.position, data.radius);
  const auto ent = coordinator.createEntity(EntityKind::ASTEROID);
  coordinator.addDbId(ent, asteroid);
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
