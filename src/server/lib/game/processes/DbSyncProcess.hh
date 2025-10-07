
#pragma once

#include "AbstractProcess.hh"
#include "DatabaseSynchronizer.hh"
#include "Entity.hh"

namespace bsgo {

class DbSyncProcess : public AbstractProcess
{
  public:
  DbSyncProcess(const Repositories &repositories);
  ~DbSyncProcess() override = default;

  void update(Coordinator &coordinator, const TickData &data) const override;

  private:
  DatabaseSynchronizer m_synchronizer;

  void updateEntity(Entity &entity, Coordinator &coordinator, const TickData &data) const;
};

} // namespace bsgo
