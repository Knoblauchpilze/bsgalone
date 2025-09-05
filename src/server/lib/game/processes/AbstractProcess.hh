
#pragma once

#include "IProcess.hh"
#include "Repositories.hh"

namespace bsgo {

class Coordinator;

class AbstractProcess : public IProcess
{
  public:
  AbstractProcess(const ProcessType &type, const Repositories &repositories);
  ~AbstractProcess() override = default;

  auto type() const -> ProcessType override;

  protected:
  Repositories m_repositories{};

  private:
  ProcessType m_processType{};
};

} // namespace bsgo
