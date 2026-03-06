
#pragma once

#include "IProcess.hh"
#include "Repositories.hh"

namespace bsgalone::server {

class AbstractProcess : public IProcess
{
  public:
  AbstractProcess(const ProcessType &type, const core::Repositories &repositories);
  ~AbstractProcess() override = default;

  auto type() const -> ProcessType override;

  protected:
  core::Repositories m_repositories{};

  private:
  ProcessType m_processType{};
};

} // namespace bsgalone::server
