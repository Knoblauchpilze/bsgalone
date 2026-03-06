
#pragma once

#include "CoreObject.hh"
#include "IMessageQueue.hh"
#include "IProcess.hh"
#include "Uuid.hh"
#include <memory>
#include <vector>

namespace bsgalone::server {

class Processes : public ::core::CoreObject
{
  public:
  Processes(const core::Uuid systemDbId, core::IMessageQueue *const systemMessageQueue);
  ~Processes() override = default;

  void update(core::Coordinator &coordinator, const chrono::TickData &data) const;

  private:
  std::vector<IProcessPtr> m_processes{};

  void initialize(const core::Uuid systemDbId, core::IMessageQueue *const systemMessageQueue);
};

using ProcessesPtr = std::unique_ptr<Processes>;

} // namespace bsgalone::server
