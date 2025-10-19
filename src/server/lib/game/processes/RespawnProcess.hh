
#pragma once

#include "AbstractProcess.hh"

namespace bsgo {

class RespawnProcess : public AbstractProcess
{
  public:
  RespawnProcess(const Repositories &repositories);
  ~RespawnProcess() override = default;

  void update(Coordinator &coordinator, const chrono::TickData &data) override;

  private:
  void respawnAsteroids(Coordinator &coordinator) const;
};

} // namespace bsgo
