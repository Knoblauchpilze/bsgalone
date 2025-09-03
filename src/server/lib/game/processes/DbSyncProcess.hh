
#pragma once

#include "AbstractProcess.hh"

namespace bsgo {

class DbSyncProcess : public AbstractProcess
{
  public:
  DbSyncProcess();
  ~DbSyncProcess() override = default;

  void update(Coordinator &coordinator, const float elapsedSeconds) const override;

  private:
};

} // namespace bsgo
