
#include "NetworkSyncComponent.hh"

namespace bsgo {

NetworkSyncComponent::NetworkSyncComponent(const std::unordered_set<ComponentType> &toSync)
  : SyncComponent(ComponentType::NETWORK_SYNC)
  , m_componentsToSync(toSync)
{}

auto NetworkSyncComponent::componentsToSync() const -> const std::unordered_set<ComponentType> &
{
  return m_componentsToSync;
}

} // namespace bsgo
