
#include "AbstractProcess.hh"

namespace bsgalone::server {

AbstractProcess::AbstractProcess(const ProcessType &type, const core::Repositories &repositories)
  : IProcess(str(type))
  , m_repositories(repositories)
  , m_processType(type)
{}

auto AbstractProcess::type() const -> ProcessType
{
  return m_processType;
}

} // namespace bsgalone::server
