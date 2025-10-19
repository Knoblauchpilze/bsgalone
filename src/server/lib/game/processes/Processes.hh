
#pragma once

#include "CoreObject.hh"
#include "IProcess.hh"
#include "Uuid.hh"
#include <memory>
#include <vector>

namespace bsgo {

class Processes : public core::CoreObject
{
  public:
  Processes(const Uuid systemDbId);
  ~Processes() override = default;

  void update(Coordinator &coordinator, const chrono::TickData &data) const;

  private:
  std::vector<IProcessPtr> m_processes{};

  void initialize(const Uuid systemDbId);
};

using ProcessesPtr = std::unique_ptr<Processes>;

} // namespace bsgo
