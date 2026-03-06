
#include "IProcess.hh"

namespace bsgalone::server {

IProcess::IProcess(const std::string &name)
  : ::core::CoreObject(name)
{
  setService("process");
}

} // namespace bsgalone::server
