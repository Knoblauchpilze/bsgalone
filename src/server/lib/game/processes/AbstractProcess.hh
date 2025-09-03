
#pragma once

#include "IProcess.hh"

namespace bsgo {

class Coordinator;

class AbstractProcess : public IProcess
{
  public:
  AbstractProcess(const ProcessType &type);
  ~AbstractProcess() override = default;

  auto type() const -> ProcessType override;

  private:
  ProcessType m_processType{};
};

} // namespace bsgo
