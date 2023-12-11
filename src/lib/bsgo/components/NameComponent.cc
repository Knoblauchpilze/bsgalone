
#include "NameComponent.hh"

namespace bsgo {

NameComponent::NameComponent(const std::string &name)
  : IComponent("name")
  , m_name(name)
{}

auto NameComponent::name() const noexcept -> std::string
{
  return m_name;
}

void NameComponent::update(const float /*elapsedSeconds*/) {}

} // namespace bsgo
