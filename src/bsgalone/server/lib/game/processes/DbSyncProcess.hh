
#pragma once

#include "AbstractProcess.hh"
#include "DatabaseSynchronizer.hh"
#include "Entity.hh"

namespace bsgalone::server {

class DbSyncProcess : public AbstractProcess
{
  public:
  DbSyncProcess(const core::Repositories &repositories);
  ~DbSyncProcess() override = default;

  void update(core::Coordinator &coordinator, const chrono::TickData &data) override;

  private:
  DatabaseSynchronizer m_synchronizer;

  void updateEntity(core::Entity &entity,
                    core::Coordinator &coordinator,
                    const chrono::TickData &data) const;
};

} // namespace bsgalone::server
