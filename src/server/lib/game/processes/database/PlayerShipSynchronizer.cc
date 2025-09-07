
#include "PlayerShipSynchronizer.hh"

namespace bsgo {

void syncPlayerShip(const Entity &entity, const Repositories &repositories)
{
  const auto dbId = entity.dbComp().dbId();

  PlayerShip ship{
    .id = dbId,
    // TODO: Correctly fetch this information
    // We also need the jump system
    .ship        = 0,
    .name        = entity.nameComp().name(),
    .player      = 0,
    .active      = true,
    .hullPoints  = entity.healthComp().value(),
    .powerPoints = entity.powerComp().value(),
  };

  repositories.playerShipRepository->save(ship);
}

} // namespace bsgo
