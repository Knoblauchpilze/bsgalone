
#include "IProcess.hh"

namespace bsgo {

IProcess::IProcess(const std::string &name)
  : core::CoreObject(name)
{
  setService("process");
}

} // namespace bsgo
