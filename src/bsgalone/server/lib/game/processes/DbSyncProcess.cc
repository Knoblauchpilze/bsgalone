
#include "DbSyncProcess.hh"
#include "Coordinator.hh"

namespace bsgalone::server {

DbSyncProcess::DbSyncProcess(const core::Repositories &repositories)
  : AbstractProcess(ProcessType::DB_SYNC, repositories)
  , m_synchronizer(repositories)
{}

namespace {
bool entityShouldBeSynced(const core::Entity &entity)
{
  const auto hasDbSyncComponent = entity.exists<core::DbSyncComponent>();
  const auto noStatus           = !entity.exists<core::StatusComponent>();
  const auto notDeadOrLeaving   = entity.exists<core::StatusComponent>()
                                && core::statusAllowsDbSynchronization(entity.statusComp().status());

  return hasDbSyncComponent && (noStatus || notDeadOrLeaving);
}
} // namespace

void DbSyncProcess::update(core::Coordinator &coordinator, const chrono::TickData &data)
{
  auto entities = coordinator.getEntitiesSatistying(entityShouldBeSynced);
  for (auto &entity : entities)
  {
    updateEntity(entity, coordinator, data);
  }
}

void DbSyncProcess::updateEntity(core::Entity &entity,
                                 core::Coordinator & /*coordinator*/,
                                 const chrono::TickData &data) const
{
  auto &dbSyncComp = entity.dbSyncComp();
  dbSyncComp.update(data);

  if (!dbSyncComp.needsSync())
  {
    return;
  }

  m_synchronizer.syncEntity(entity);
}

} // namespace bsgalone::server
