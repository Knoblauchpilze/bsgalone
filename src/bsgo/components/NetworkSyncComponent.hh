

#pragma once

#include "SyncComponent.hh"
#include <unordered_set>

namespace bsgo {

class NetworkSyncComponent : public SyncComponent
{
  public:
  NetworkSyncComponent(const std::unordered_set<ComponentType> &toSync);
  ~NetworkSyncComponent() override = default;

  auto componentsToSync() const -> const std::unordered_set<ComponentType> &;

  private:
  std::unordered_set<ComponentType> m_componentsToSync{};
};

using NetworkSyncComponentShPtr = std::shared_ptr<NetworkSyncComponent>;

} // namespace bsgo
