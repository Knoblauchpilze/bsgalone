
#include "PlayerShipSynchronizer.hh"

namespace bsgalone::server {

void syncPlayerShip(const core::Entity &entity, const core::Repositories &repositories)
{
  const auto dbId = entity.dbComp().dbId();

  core::PlayerShip ship{
    .dbId        = dbId,
    .name        = entity.nameComp().name(),
    .active      = true,
    .hullPoints  = entity.healthComp().value(),
    .powerPoints = entity.powerComp().value(),
    .position    = entity.transformComp().position(),
  };

  repositories.playerShipRepository->save(ship);
}

} // namespace bsgalone::server
