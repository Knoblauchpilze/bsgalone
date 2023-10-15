
#include "IComponent.hh"

namespace bsgo {

IComponent::IComponent(const std::string &name)
  : utils::CoreObject(name)
{
  setService("component");
}

} // namespace bsgo
