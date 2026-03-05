
#include "PlayerShipSynchronizer.hh"

namespace bsgo {

void syncPlayerShip(const Entity &entity, const Repositories &repositories)
{
  const auto dbId = entity.dbComp().dbId();

  PlayerShip ship{
    .id          = dbId,
    .name        = entity.nameComp().name(),
    .active      = true,
    .hullPoints  = entity.healthComp().value(),
    .powerPoints = entity.powerComp().value(),
    .position    = entity.transformComp().position(),
  };

  repositories.playerShipRepository->save(ship);
}

} // namespace bsgo
