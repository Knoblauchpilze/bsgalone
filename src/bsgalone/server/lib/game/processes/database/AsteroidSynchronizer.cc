
#include "AsteroidSynchronizer.hh"

namespace bsgalone::server {

void syncAsteroid(const core::Entity &entity, const core::Repositories &repositories)
{
  const auto dbId        = entity.dbComp().dbId();
  const auto &healthComp = entity.healthComp();

  core::Asteroid asteroid{.id = dbId, .health = healthComp.value()};
  repositories.asteroidRepository->save(asteroid);
}

} // namespace bsgalone::server
