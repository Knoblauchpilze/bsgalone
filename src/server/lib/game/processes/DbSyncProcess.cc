
#include "DbSyncProcess.hh"
#include "Coordinator.hh"

namespace bsgo {

DbSyncProcess::DbSyncProcess(const Repositories &repositories)
  : AbstractProcess(ProcessType::DB_SYNC, repositories)
  , m_synchronizer(repositories)
{}

namespace {
bool entityShouldBeSynced(const Entity &entity)
{
  const auto hasDbSyncComponent = entity.exists<DbSyncComponent>();
  const auto noStatus           = !entity.exists<StatusComponent>();
  const auto notDeadOrLeaving   = entity.exists<StatusComponent>()
                                && statusAllowsDbSynchronization(entity.statusComp().status());

  return hasDbSyncComponent && (noStatus || notDeadOrLeaving);
}
} // namespace

void DbSyncProcess::update(Coordinator &coordinator, const float elapsedSeconds) const
{
  auto entities = coordinator.getEntitiesSatistying(entityShouldBeSynced);
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
