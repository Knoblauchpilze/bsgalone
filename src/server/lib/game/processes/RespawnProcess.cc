
#include "RespawnProcess.hh"

namespace bsgo {

RespawnProcess::RespawnProcess()
  : AbstractProcess(ProcessType::RESPAWN)
{}

void RespawnProcess::update(Coordinator & /*coordinator*/, const float /*elapsedSeconds*/) const
{
  // TODO: Handle process update
  warn("Should handle process update");
}

} // namespace bsgo
