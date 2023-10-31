
#include "OwnerComponent.hh"

namespace bsgo {

OwnerComponent::OwnerComponent(const Uuid &owner)
  : IComponent("owner")
  , m_owner(owner)
{}

auto OwnerComponent::owner() const -> Uuid
{
  return m_owner;
}

void OwnerComponent::update(const float /*elapsedSeconds*/) {}

} // namespace bsgo
