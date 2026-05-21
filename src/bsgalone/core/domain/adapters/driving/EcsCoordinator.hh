
#pragma once

#include "EntityRegistry.hh"
#include "ForManagingHealth.hh"
#include "ForRunningSimulation.hh"

namespace bsgalone::core {

class EcsCoordinator : public ForRunningSimulation
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
