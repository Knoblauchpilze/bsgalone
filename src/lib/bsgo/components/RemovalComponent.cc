
#include "RemovalComponent.hh"

namespace bsgo {

RemovalComponent::RemovalComponent()
  : AbstractComponent(ComponentType::REMOVAL)
{}

void RemovalComponent::markForRemoval(const bool toRemove)
{
  m_markedForRemoval = toRemove;
}

bool RemovalComponent::toBeDeleted() const
{
  return m_markedForRemoval;
}

void RemovalComponent::update(const float /*elapsedSeconds*/) {}

} // namespace bsgo
