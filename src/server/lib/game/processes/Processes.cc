
#include "Processes.hh"
#include "Repositories.hh"

#include "DbSyncProcess.hh"
#include "RespawnProcess.hh"
#include "TickSyncProcess.hh"

namespace bsgo {

Processes::Processes(const Uuid systemDbId, IMessageQueue *const systemMessageQueue)
  : core::CoreObject("processes")
{
  setService("bsgo");

  initialize(systemDbId, systemMessageQueue);
}

void Processes::update(Coordinator &coordinator, const chrono::TickData &data) const
{
  for (const auto &process : m_processes)
  {
    process->update(coordinator, data);
  }
}

void Processes::initialize(const Uuid systemDbId, IMessageQueue *const systemMessageQueue)
{
  Repositories repositories;

  auto syncProcess = std::make_unique<DbSyncProcess>(repositories);
  m_processes.emplace_back(std::move(syncProcess));
  auto respawnProcess = std::make_unique<RespawnProcess>(systemDbId,
                                                         repositories,
                                                         systemMessageQueue);
  m_processes.emplace_back(std::move(respawnProcess));
  auto tickSyncProcess = std::make_unique<TickSyncProcess>(systemDbId, repositories);
  m_processes.emplace_back(std::move(tickSyncProcess));
}

} // namespace bsgo
