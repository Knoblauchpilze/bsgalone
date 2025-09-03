
#include "Processes.hh"

#include "DbSyncProcess.hh"
#include "RespawnProcess.hh"

namespace bsgo {

Processes::Processes()
  : core::CoreObject("processes")
{
  setService("bsgo");

  initialize();
}

void Processes::update(Coordinator &coordinator, const float elapsedSeconds) const
{
  for (const auto &process : m_processes)
  {
    process->update(coordinator, elapsedSeconds);
  }
}

namespace {
template<typename T>
void createProcess(std::vector<IProcessPtr> &processes)
{
  processes.emplace_back(std::make_unique<T>());
}
} // namespace

void Processes::initialize()
{
  createProcess<DbSyncProcess>(m_processes);
  createProcess<RespawnProcess>(m_processes);
}

} // namespace bsgo
