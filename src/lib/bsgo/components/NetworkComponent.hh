

#pragma once

#include "AbstractComponent.hh"
#include <unordered_set>

namespace bsgo {

class NetworkComponent : public AbstractComponent
{
  public:
  NetworkComponent(const std::unordered_set<ComponentType> &toSync);
  ~NetworkComponent() override = default;

  bool needsSync() const;
  void markForSync(const bool needsSync = true);

  auto componentsToSync() const -> const std::unordered_set<ComponentType> &;

  void update(const float elapsedSeconds) override;

  private:
  bool m_needsSync{false};
  std::unordered_set<ComponentType> m_componentsToSync{};
};

using NetworkComponentShPtr = std::shared_ptr<NetworkComponent>;

} // namespace bsgo
