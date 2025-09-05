

#pragma once

#include "SyncComponent.hh"
#include <unordered_set>

namespace bsgo {

class DbSyncComponent : public SyncComponent
{
  public:
  DbSyncComponent(const std::unordered_set<ComponentType> &toSync);
  ~DbSyncComponent() override = default;

  auto componentsToSync() const -> const std::unordered_set<ComponentType> &;

  private:
  std::unordered_set<ComponentType> m_componentsToSync{};
};

using DbSyncComponentShPtr = std::shared_ptr<DbSyncComponent>;

} // namespace bsgo
