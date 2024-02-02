
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

void AsteroidDataSource::initialize(Coordinator &coordinator,
                                    DatabaseEntityMapper &entityMapper) const
{
  const auto asteroids = m_repositories.systemRepository->findAllAsteroidsBySystem(m_systemDbId);
  for (const auto &id : asteroids)
  {
    registerAsteroid(coordinator, id, entityMapper);
  }
}

void AsteroidDataSource::registerAsteroid(Coordinator &coordinator,
                                          const Uuid asteroidDbId,
                                          DatabaseEntityMapper &entityMapper) const
{
  const auto data = m_repositories.asteroidRepository->findOneById(asteroidDbId);

  auto box                    = std::make_unique<CircleBox>(data.position, data.radius);
  const auto asteroidEntityId = coordinator.createEntity(EntityKind::ASTEROID);
  coordinator.addDbId(asteroidEntityId, asteroidDbId);
  coordinator.addTransform(asteroidEntityId, std::move(box));
  coordinator.addHealth(asteroidEntityId, data.health, data.health, 0.0f);
  coordinator.addRemoval(asteroidEntityId);
  coordinator.addScanned(asteroidEntityId);
  if (data.loot)
  {
    coordinator.addLoot(asteroidEntityId);
    const auto loot = m_repositories.asteroidLootRepository->findOneById(asteroidDbId);
    coordinator.addResourceComponent(asteroidEntityId, loot.resource, loot.amount);
  }

  entityMapper.registerAsteroid(asteroidDbId, asteroidEntityId);
}

} // namespace bsgo
