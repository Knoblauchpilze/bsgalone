
#pragma once

#include "CoreObject.hh"
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
  void clear() override;

  private:
  EntityRegistryShPtr m_entityRegistry{};
  ForManagingHealthPtr m_healthSystem{};

  void initialize();
};

} // namespace bsgalone::core
