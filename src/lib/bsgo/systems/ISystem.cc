
#include "ISystem.hh"

namespace bsgo {

ISystem::ISystem(const std::string &name)
  : utils::CoreObject(name)
{
  setService("system");
}

} // namespace bsgo
