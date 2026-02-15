
#pragma once

#include "CoreObject.hh"
#include "IMessageQueue.hh"
#include "IProcess.hh"
#include "Uuid.hh"
#include <memory>
#include <vector>

namespace bsgo {

class Processes : public core::CoreObject
{
  public:
  Processes(const Uuid systemDbId, bsgalone::core::IMessageQueue *const systemMessageQueue);
  ~Processes() override = default;

  void update(Coordinator &coordinator, const chrono::TickData &data) const;

  private:
  std::vector<IProcessPtr> m_processes{};

  void initialize(const Uuid systemDbId, bsgalone::core::IMessageQueue *const systemMessageQueue);
};

using ProcessesPtr = std::unique_ptr<Processes>;

} // namespace bsgo
