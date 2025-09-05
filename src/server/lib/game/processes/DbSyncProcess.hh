
#pragma once

#include "AbstractProcess.hh"
#include "Entity.hh"

namespace bsgo {

class DbSyncProcess : public AbstractProcess
{
  public:
  DbSyncProcess();
  ~DbSyncProcess() override = default;

  void update(Coordinator &coordinator, const float elapsedSeconds) const override;

  private:
  void updateEntity(Entity &entity, Coordinator &coordinator, const float elapsedSeconds) const;
  void syncEntity(Entity &entity) const;
};

} // namespace bsgo
