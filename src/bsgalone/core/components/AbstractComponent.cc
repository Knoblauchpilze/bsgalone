
#include "AbstractComponent.hh"

namespace bsgalone::core {

AbstractComponent::AbstractComponent(const ComponentType &type)
  : IComponent()
  , ::core::CoreObject(str(type))
  , m_componentType(type)
{
  setService("component");
}

auto AbstractComponent::type() const -> ComponentType
{
  return m_componentType;
}

} // namespace bsgalone::core
