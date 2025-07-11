
#include "AsteroidDataSource.hh"
#include "CircleBox.hh"
#include "Coordinator.hh"

namespace bsgo {

AsteroidDataSource::AsteroidDataSource()
  : core::CoreObject("bsgo")
{
  setService("data");
  addModule("asteroid");
}

AsteroidDataSource::AsteroidDataSource(const Repositories &repositories)
  : core::CoreObject("bsgo")
  , m_repositories(repositories)
{
  setService("data");
  addModule("asteroid");
}

void AsteroidDataSource::initialize(const Uuid systemDbId,
                                    Coordinator &coordinator,
                                    DatabaseEntityMapper &entityMapper) const
{
  if (!m_repositories)
  {
    error("Failed to initialize asteroid", "Repositories are not set");
  }

  const auto asteroids = m_repositories->systemRepository->findAllAsteroidsBySystem(systemDbId);
  for (const auto &id : asteroids)
  {
    registerAsteroid(coordinator, id, entityMapper);
  }
}

void AsteroidDataSource::registerAsteroid(Coordinator &coordinator,
                                          const AsteroidData &data,
                                          DatabaseEntityMapper &entityMapper) const

{
  const auto asteroidEntityId = coordinator.createEntity(EntityKind::ASTEROID);

  coordinator.addDbId(asteroidEntityId, data.dbId);
  auto box = std::make_unique<CircleBox>(data.position, data.radius);
  coordinator.addTransform(asteroidEntityId, std::move(box));
  coordinator.addHealth(asteroidEntityId, data.health, data.health, 0.0f);
  coordinator.addRemoval(asteroidEntityId);
  coordinator.addScanned(asteroidEntityId);
  if (data.resource && data.amount)
  {
    coordinator.addLoot(asteroidEntityId);
    coordinator.addResourceComponent(asteroidEntityId, *data.resource, *data.amount);
  }
  coordinator.addNetwork(asteroidEntityId, {ComponentType::HEALTH});

  entityMapper.registerAsteroid(data.dbId, asteroidEntityId);
}

void AsteroidDataSource::registerAsteroid(Coordinator &coordinator,
                                          const Uuid asteroidDbId,
                                          DatabaseEntityMapper &entityMapper) const
{
  const auto data = m_repositories->asteroidRepository->findOneById(asteroidDbId);

  AsteroidData out{
    .dbId     = asteroidDbId,
    .position = data.position,
    .radius   = data.radius,
    .health   = data.health,
  };

  if (data.loot)
  {
    const auto loot = m_repositories->asteroidLootRepository->findOneById(asteroidDbId);
    out.resource    = loot.resource;
    out.amount      = loot.amount;
  }

  registerAsteroid(coordinator, out, entityMapper);
}

} // namespace bsgo
