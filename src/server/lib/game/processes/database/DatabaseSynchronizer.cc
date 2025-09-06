
#include "DatabaseSynchronizer.hh"
#include "HealthSynchronizer.hh"

namespace bsgo {

DatabaseSynchronizer::DatabaseSynchronizer(const Repositories &repositories)
  : core::CoreObject("synchronizer")
  , m_repositories(repositories)
{
  setService("database");
}

void DatabaseSynchronizer::syncComponent(const Entity &entity, const ComponentType type) const
{
  switch (type)
  {
    case ComponentType::HEALTH:
      syncHealthComponent(entity, m_repositories);
      break;
    default:
      error("Unsupported component " + str(type) + " to sync");
      break;
  }
}

} // namespace bsgo
