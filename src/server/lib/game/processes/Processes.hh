
#pragma once

#include "CoreObject.hh"
#include "IProcess.hh"
#include <memory>
#include <vector>

namespace bsgo {

class Processes : public core::CoreObject
{
  public:
  Processes();
  ~Processes() override = default;

  void update(Coordinator &coordinator, const TickData &data) const;

  private:
  std::vector<IProcessPtr> m_processes{};

  void initialize();
};

using ProcessesPtr = std::unique_ptr<Processes>;

} // namespace bsgo
