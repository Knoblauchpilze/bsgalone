
#include "NetworkSyncComponent.hh"

namespace bsgo {

NetworkSyncComponent::NetworkSyncComponent(
  const std::unordered_set<bsgalone::core::ComponentType> &toSync)
  : SyncComponent(bsgalone::core::ComponentType::NETWORK_SYNC)
  , m_componentsToSync(toSync)
{}

auto NetworkSyncComponent::componentsToSync() const
  -> const std::unordered_set<bsgalone::core::ComponentType> &
{
  return m_componentsToSync;
}

} // namespace bsgo
