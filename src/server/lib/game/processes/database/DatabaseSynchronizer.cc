
#include "DatabaseSynchronizer.hh"
#include "AsteroidSynchronizer.hh"
#include "PlayerShipSynchronizer.hh"
#include "SystemOutpostSynchronizer.hh"

namespace bsgo {

DatabaseSynchronizer::DatabaseSynchronizer(const Repositories &repositories)
  : core::CoreObject("synchronizer")
  , m_repositories(repositories)
{
  setService("database");
}

void DatabaseSynchronizer::syncEntity(Entity &entity) const
{
  const auto kind = entity.kind->kind();

  switch (kind)
  {
    case EntityKind::ASTEROID:
      syncAsteroid(entity, m_repositories);
      break;
    case EntityKind::SHIP:
      syncPlayerShip(entity, m_repositories);
      break;
    case EntityKind::OUTPOST:
      syncSystemOutpost(entity, m_repositories);
      break;
    default:
      error("Unsupported entity kind " + str(kind) + " to sync");
      break;
  }

  entity.dbSyncComp().markAsJustSynced();
}

} // namespace bsgo
