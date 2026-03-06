
#pragma once

#include "AbstractProcess.hh"

namespace bsgalone::server {

class TickSyncProcess : public AbstractProcess
{
  public:
  TickSyncProcess(const core::Uuid systemDbId, const core::Repositories &repositories);
  ~TickSyncProcess() override = default;

  void update(core::Coordinator &coordinator, const chrono::TickData &data) override;

  private:
  core::Uuid m_systemDbId{};
  std::optional<int> m_lastSaved{};

  bool shouldSyncTick(const chrono::Tick &tick) const;
};

} // namespace bsgalone::server
