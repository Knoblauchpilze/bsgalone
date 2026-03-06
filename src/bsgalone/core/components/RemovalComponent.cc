
#include "RemovalComponent.hh"

namespace bsgalone::core {

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

void RemovalComponent::update(const chrono::TickData & /*data*/) {}

} // namespace bsgalone::core
