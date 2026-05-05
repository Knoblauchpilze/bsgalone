
#pragma once

#include "EntityRegistry.hh"
#include "ForManagingHealth.hh"
#include "IEcsCoordinator.hh"

namespace bsgalone::core {

class EcsCoordinator : public IEcsCoordinator
{
  public:
  EcsCoordinator(EntityRegistryShPtr entityRegistry);
  ~EcsCoordinator() override = default;

  void update(const chrono::TickData &data) override;

  private:
  ForManagingHealthPtr m_healthSystem{};

  void initialize(EntityRegistryShPtr entityRegistry);
};

} // namespace bsgalone::core
