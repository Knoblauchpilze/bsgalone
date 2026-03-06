
#include "SystemOutpostSynchronizer.hh"

namespace bsgalone::server {

void syncSystemOutpost(const core::Entity &entity, const core::Repositories &repositories)
{
  const auto dbId = entity.dbComp().dbId();

  core::SystemOutpost outpost{
    .id          = dbId,
    .hullPoints  = entity.healthComp().value(),
    .powerPoints = entity.powerComp().value(),
  };

  repositories.systemOutpostRepository->save(outpost);
}

} // namespace bsgalone::server
