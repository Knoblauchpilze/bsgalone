
#include "DbSyncProcess.hh"

namespace bsgo {

DbSyncProcess::DbSyncProcess()
  : AbstractProcess(ProcessType::DB_SYNC)
{}

void DbSyncProcess::update(Coordinator & /*coordinator*/, const float /*elapsedSeconds*/) const
{
  // TODO: Handle process update
  warn("Should handle process update");
}

} // namespace bsgo
