
#include "DbSyncProcess.hh"
#include "Coordinator.hh"

namespace bsgo {

DbSyncProcess::DbSyncProcess(const Repositories &repositories)
  : AbstractProcess(ProcessType::DB_SYNC, repositories)
  , m_synchronizer(repositories)
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

  m_synchronizer.syncEntity(entity);
}

} // namespace bsgo
