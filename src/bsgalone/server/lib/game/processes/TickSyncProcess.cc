
#include "TickSyncProcess.hh"

namespace bsgalone::server {

TickSyncProcess::TickSyncProcess(const core::Uuid systemDbId, const core::Repositories &repositories)
  : AbstractProcess(ProcessType::TICK_SYNC, repositories)
  , m_systemDbId(systemDbId)
{}

void TickSyncProcess::update(core::Coordinator & /*coordinator*/, const chrono::TickData &data)
{
  if (!shouldSyncTick(data.tick))
  {
    return;
  }

  core::SystemTick tick{.system = m_systemDbId, .currentTick = data.tick};

  m_repositories.tickRepository->save(tick);
  m_lastSaved = data.tick.count();
}

bool TickSyncProcess::shouldSyncTick(const chrono::Tick &tick) const
{
  return !m_lastSaved || *m_lastSaved != tick.count();
}

} // namespace bsgalone::server
