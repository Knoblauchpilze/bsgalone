
#include "AsteroidSynchronizer.hh"

namespace bsgo {

void syncAsteroid(const Entity &entity, const Repositories &repositories)
{
  const auto dbId        = entity.dbComp().dbId();
  const auto &healthComp = entity.healthComp();

  Asteroid asteroid{.id = dbId, .health = healthComp.value()};
  repositories.asteroidRepository->save(asteroid);
}

} // namespace bsgo
