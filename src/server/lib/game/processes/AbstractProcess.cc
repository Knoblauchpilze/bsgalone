
#include "AbstractProcess.hh"

namespace bsgo {

AbstractProcess::AbstractProcess(const ProcessType &type)
  : IProcess(str(type))
  , m_processType(type)
{}

auto AbstractProcess::type() const -> ProcessType
{
  return m_processType;
}

} // namespace bsgo
