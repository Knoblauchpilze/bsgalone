
#include "TickSyncProcess.hh"

namespace bsgo {

TickSyncProcess::TickSyncProcess(const Uuid systemDbId, const Repositories &repositories)
  : AbstractProcess(ProcessType::TICK_SYNC, repositories)
  , m_systemDbId(systemDbId)
{}

void TickSyncProcess::update(Coordinator & /*coordinator*/, const chrono::TickData &data)
{
  if (!shouldSyncTick(data.tick))
  {
    return;
  }

  SystemTick tick{.system = m_systemDbId, .currentTick = data.tick};
  info("saving tick " + tick.currentTick.str());

  m_repositories.tickRepository->save(tick);
  m_lastSaved = data.tick.count();
}

bool TickSyncProcess::shouldSyncTick(const chrono::Tick &tick) const
{
  return !m_lastSaved || *m_lastSaved != tick.count();
}

} // namespace bsgo
