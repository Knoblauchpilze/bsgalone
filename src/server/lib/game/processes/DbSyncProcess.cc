
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

void DbSyncProcess::update(Coordinator &coordinator, const TickData &data) const
{
  auto entities = coordinator.getEntitiesSatistying(entityShouldBeSynced);
  for (auto &entity : entities)
  {
    updateEntity(entity, coordinator, data);
  }
}

void DbSyncProcess::updateEntity(Entity &entity,
                                 Coordinator & /*coordinator*/,
                                 const TickData &data) const
{
  auto &dbSyncComp = entity.dbSyncComp();
  // TODO: We should use the tick duration as is.
  dbSyncComp.update(data.elapsed.toSeconds());

  if (!dbSyncComp.needsSync())
  {
    return;
  }

  m_synchronizer.syncEntity(entity);
}

} // namespace bsgo
