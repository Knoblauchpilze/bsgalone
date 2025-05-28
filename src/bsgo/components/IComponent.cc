
#include "IComponent.hh"

namespace bsgo {

IComponent::IComponent(const std::string &name)
  : core::CoreObject(name)
{
  setService("component");
}

} // namespace bsgo
