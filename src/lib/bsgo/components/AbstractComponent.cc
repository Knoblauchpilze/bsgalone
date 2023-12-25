
#include "AbstractComponent.hh"

namespace bsgo {

AbstractComponent::AbstractComponent(const std::string &name, const ComponentType &type)
  : IComponent(name)
  , m_componentType(type)
{}

auto AbstractComponent::type() const -> ComponentType
{
  return m_componentType;
}

} // namespace bsgo
