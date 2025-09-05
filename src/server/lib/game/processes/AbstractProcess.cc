
#include "AbstractProcess.hh"

namespace bsgo {

AbstractProcess::AbstractProcess(const ProcessType &type, const Repositories &repositories)
  : IProcess(str(type))
  , m_repositories(repositories)
  , m_processType(type)
{}

auto AbstractProcess::type() const -> ProcessType
{
  return m_processType;
}

} // namespace bsgo
