
#include "DbSyncComponent.hh"

namespace bsgo {

DbSyncComponent::DbSyncComponent(const std::unordered_set<ComponentType> &toSync)
  : SyncComponent(ComponentType::DB_SYNC)
  , m_componentsToSync(toSync)
{}

auto DbSyncComponent::componentsToSync() const -> const std::unordered_set<ComponentType> &
{
  return m_componentsToSync;
}

} // namespace bsgo
