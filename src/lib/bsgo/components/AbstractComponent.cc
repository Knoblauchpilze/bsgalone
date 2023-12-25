
#include "AbstractComponent.hh"

namespace bsgo {

AbstractComponent::AbstractComponent(const ComponentType &type)
  : IComponent(str(type))
  , m_componentType(type)
{}

auto AbstractComponent::type() const -> ComponentType
{
  return m_componentType;
}

} // namespace bsgo
