
#include "HealthSynchronizer.hh"

namespace bsgo {

void syncSystemOutpostHealth(const Repositories &repositories,
                             const Uuid outpostDbId,
                             const HealthComponent &healthComp)
{
  SystemOutpost outpost{.id = outpostDbId, .hullPoints = healthComp.value()};
  repositories.systemOutpostRepository->save(outpost);
}

void syncAsteroidHealth(const Repositories &repositories,
                        const Uuid asteroidDbId,
                        const HealthComponent &healthComp)
{
  Asteroid asteroid{.id = asteroidDbId, .health = healthComp.value()};
  repositories.asteroidRepository->save(asteroid);
}

void syncHealthComponent(const Entity &entity, const Repositories &repositories)
{
  const auto dbId        = entity.dbComp().dbId();
  const auto &healthComp = entity.healthComp();

  switch (entity.kind->kind())
  {
    case EntityKind::ASTEROID:
      syncAsteroidHealth(repositories, dbId, healthComp);
      break;
    case EntityKind::OUTPOST:
      syncSystemOutpostHealth(repositories, dbId, healthComp);
      break;
    default:
      throw std::invalid_argument("Unsupported entity with kind " + str(entity.kind->type())
                                  + " for health sync");
  }
}

} // namespace bsgo
