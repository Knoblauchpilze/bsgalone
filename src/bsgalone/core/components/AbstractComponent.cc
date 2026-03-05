
#include "AbstractComponent.hh"

namespace bsgo {

AbstractComponent::AbstractComponent(const bsgalone::core::ComponentType &type)
  : IComponent()
  , ::core::CoreObject(str(type))
  , m_componentType(type)
{
  setService("component");
}

auto AbstractComponent::type() const -> bsgalone::core::ComponentType
{
  return m_componentType;
}

} // namespace bsgo
