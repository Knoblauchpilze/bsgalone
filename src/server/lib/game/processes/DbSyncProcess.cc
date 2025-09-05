
#include "DbSyncProcess.hh"
#include "Coordinator.hh"

namespace bsgo {

DbSyncProcess::DbSyncProcess()
  : AbstractProcess(ProcessType::DB_SYNC)
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

void DbSyncProcess::syncEntity(Entity &entity) const
{
  auto &dbSyncComp = entity.dbSyncComp();

  // TODO: Handle process update
  warn("Should sync entity " + str(entity.uuid));

  dbSyncComp.markAsJustSynced();
}

} // namespace bsgo
