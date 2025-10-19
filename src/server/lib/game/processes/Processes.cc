
#include "Processes.hh"
#include "Repositories.hh"

#include "DbSyncProcess.hh"
#include "RespawnProcess.hh"
#include "TickSyncProcess.hh"

namespace bsgo {

Processes::Processes(const Uuid systemDbId)
  : core::CoreObject("processes")
{
  setService("bsgo");

  initialize(systemDbId);
}

void Processes::update(Coordinator &coordinator, const chrono::TickData &data) const
{
  for (const auto &process : m_processes)
  {
    process->update(coordinator, data);
  }
}

namespace {
template<typename T>
void createProcess(std::vector<IProcessPtr> &processes, const Repositories &repositories)
{
  processes.emplace_back(std::make_unique<T>(repositories));
}
} // namespace

void Processes::initialize(const Uuid systemDbId)
{
  Repositories repositories;

  createProcess<DbSyncProcess>(m_processes, repositories);
  createProcess<RespawnProcess>(m_processes, repositories);
  m_processes.emplace_back(std::make_unique<TickSyncProcess>(systemDbId, repositories));
}

} // namespace bsgo
