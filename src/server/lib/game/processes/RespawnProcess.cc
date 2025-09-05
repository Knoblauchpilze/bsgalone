
#include "RespawnProcess.hh"

namespace bsgo {

RespawnProcess::RespawnProcess(const Repositories &repositories)
  : AbstractProcess(ProcessType::RESPAWN, repositories)
{}

void RespawnProcess::update(Coordinator & /*coordinator*/, const float /*elapsedSeconds*/) const
{
  // TODO: Handle process update
  warn("Should handle process update");
}

} // namespace bsgo
