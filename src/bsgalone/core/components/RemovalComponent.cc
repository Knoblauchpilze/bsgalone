
#include "RemovalComponent.hh"

namespace bsgo {

RemovalComponent::RemovalComponent()
  : AbstractComponent(bsgalone::core::ComponentType::REMOVAL)
{}

void RemovalComponent::markForRemoval(const bool toRemove)
{
  m_markedForRemoval = toRemove;
}

bool RemovalComponent::toBeDeleted() const
{
  return m_markedForRemoval;
}

void RemovalComponent::update(const chrono::TickData & /*data*/) {}

} // namespace bsgo
