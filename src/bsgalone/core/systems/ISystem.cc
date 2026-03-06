
#include "ISystem.hh"

namespace bsgalone::core {

ISystem::ISystem(const std::string &name)
  : ::core::CoreObject(name)
{
  setService("system");
}

} // namespace bsgalone::core
