
#include "OwnerComponent.hh"

namespace bsgo {

OwnerComponent::OwnerComponent(const Uuid &owner, const OwnerType &ownerType)
  : IComponent("owner")
  , m_owner(owner)
  , m_ownerType(ownerType)
{}

auto OwnerComponent::owner() const -> Uuid
{
  return m_owner;
}

auto OwnerComponent::category() const -> OwnerType
{
  return m_ownerType;
}

void OwnerComponent::update(const float /*elapsedSeconds*/) {}

} // namespace bsgo
