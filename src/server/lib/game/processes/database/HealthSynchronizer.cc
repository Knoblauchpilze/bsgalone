
#include "HealthSynchronizer.hh"

namespace bsgo {

void syncAsteroidHealth(const Repositories &repositories,
                        const Uuid asteroidDbId,
                        const HealthComponent &healthComp)
{
  Asteroid asteroid{.id = asteroidDbId, .health = healthComp.value()};
  repositories.asteroidRepository->save(asteroid);
}

void syncHealthComponent(const Entity &entity, const Repositories &repositories)
{
  switch (entity.kind->kind())
  {
    case EntityKind::ASTEROID:
      syncAsteroidHealth(repositories, entity.dbComp().dbId(), entity.healthComp());
      break;
    default:
      throw std::invalid_argument("Unsupported entity with kind " + str(entity.kind->type())
                                  + " for health sync");
  }
}

} // namespace bsgo
