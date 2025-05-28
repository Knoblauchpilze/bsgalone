
#include "ISystem.hh"

namespace bsgo {

ISystem::ISystem(const std::string &name)
  : core::CoreObject(name)
{
  setService("system");
}

} // namespace bsgo
