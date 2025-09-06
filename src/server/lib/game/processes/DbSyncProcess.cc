
#include "DbSyncProcess.hh"
#include "Coordinator.hh"

namespace bsgo {

DbSyncProcess::DbSyncProcess(const Repositories &repositories)
  : AbstractProcess(ProcessType::DB_SYNC, repositories)
{}

namespace {
bool entityHasDbSyncComponent(const Entity &entity)
{
  return entity.exists<DbSyncComponent>();
}
} // namespace

void DbSyncProcess::update(Coordinator &coordinator, const float elapsedSeconds) const
{
  auto entities = coordinator.getEntitiesSatistying(entityHasDbSyncComponent);
  for (auto &entity : entities)
  {
    updateEntity(entity, coordinator, elapsedSeconds);
  }
}

void DbSyncProcess::updateEntity(Entity &entity,
                                 Coordinator & /*coordinator*/,
                                 const float elapsedSeconds) const
{
  auto &dbSyncComp = entity.dbSyncComp();
  dbSyncComp.update(elapsedSeconds);

  if (!dbSyncComp.needsSync())
  {
    return;
  }

  syncEntity(entity);
}

namespace {
void syncAsteroidHealth(const Repositories &repositories,
                        const Uuid asteroidDbId,
                        const HealthComponent &healthComp)
{
  Asteroid asteroid{.id = asteroidDbId, .health = healthComp.value()};
  repositories.asteroidRepository->save(asteroid);
}

void syncHealthComponent(const Entity &entity, const Repositories &repositories)
{
  switch (entity.kind->kind())
  {
    case EntityKind::ASTEROID:
      syncAsteroidHealth(repositories, entity.dbComp().dbId(), entity.healthComp());
      break;
    default:
      throw std::invalid_argument("Unsupported entity with kind " + str(entity.kind->type())
                                  + " for health sync");
  }
}

void syncComponent(const Entity &entity, const ComponentType type, const Repositories &repositories)
{
  switch (type)
  {
    case ComponentType::HEALTH:
      syncHealthComponent(entity, repositories);
      break;
    default:
      throw std::invalid_argument("Unsupported component " + str(type) + " to sync");
  }
}
} // namespace

void DbSyncProcess::syncEntity(Entity &entity) const
{
  auto &dbSyncComp = entity.dbSyncComp();

  for (const auto &comp : dbSyncComp.componentsToSync())
  {
    syncComponent(entity, comp, m_repositories);
  }

  dbSyncComp.markAsJustSynced();
}

} // namespace bsgo
