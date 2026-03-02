
#include "AbstractComponent.hh"

namespace bsgo {

AbstractComponent::AbstractComponent(const bsgalone::core::ComponentType &type)
  : IComponent(str(type))
  , m_componentType(type)
{}

auto AbstractComponent::type() const -> bsgalone::core::ComponentType
{
  return m_componentType;
}

} // namespace bsgo
