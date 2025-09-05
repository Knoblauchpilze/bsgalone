
#pragma once

#include "AbstractProcess.hh"

namespace bsgo {

class RespawnProcess : public AbstractProcess
{
  public:
  RespawnProcess(const Repositories &repositories);
  ~RespawnProcess() override = default;

  void update(Coordinator &coordinator, const float elapsedSeconds) const override;

  private:
};

} // namespace bsgo
