
#include "NetworkComponent.hh"

namespace bsgo {

NetworkComponent::NetworkComponent(const std::unordered_set<ComponentType> &toSync)
  : AbstractComponent(ComponentType::NETWORK)
  , m_componentsToSync(toSync)
{}

bool NetworkComponent::needsSync() const
{
  return m_needsSync;
}

void NetworkComponent::markForSync(const bool needsSync)
{
  m_needsSync = needsSync;
}

auto NetworkComponent::componentsToSync() const -> const std::unordered_set<ComponentType> &
{
  return m_componentsToSync;
}

void NetworkComponent::update(const float /*elapsedSeconds*/) {}

} // namespace bsgo
