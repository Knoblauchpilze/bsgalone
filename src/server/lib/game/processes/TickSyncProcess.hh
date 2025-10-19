
#pragma once

#include "AbstractProcess.hh"

namespace bsgo {

class TickSyncProcess : public AbstractProcess
{
  public:
  TickSyncProcess(const Uuid systemDbId, const Repositories &repositories);
  ~TickSyncProcess() override = default;

  void update(Coordinator &coordinator, const chrono::TickData &data) override;

  private:
  Uuid m_systemDbId{};
  std::optional<int> m_lastSaved{};

  bool shouldSyncTick(const chrono::Tick &tick) const;
};

} // namespace bsgo
