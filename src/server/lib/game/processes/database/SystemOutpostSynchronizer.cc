
#include "SystemOutpostSynchronizer.hh"

namespace bsgo {

void syncSystemOutpost(const Entity &entity, const Repositories &repositories)
{
  const auto dbId = entity.dbComp().dbId();

  SystemOutpost outpost{
    .id          = dbId,
    .hullPoints  = entity.healthComp().value(),
    .powerPoints = entity.powerComp().value(),
  };

  repositories.systemOutpostRepository->save(outpost);
}

} // namespace bsgo
