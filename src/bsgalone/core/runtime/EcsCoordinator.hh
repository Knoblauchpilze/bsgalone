
#pragma once

#include "EntityRegistry.hh"
#include "ForManagingHealth.hh"
#include "TickData.hh"

namespace bsgalone::core {

class EcsCoordinator
{
  public:
  EcsCoordinator(EntityRegistryShPtr entityRegistry);

  void update(const chrono::TickData &data);

  private:
  ForManagingHealthPtr m_healthSystem{};

  void initialize(EntityRegistryShPtr entityRegistry);
};

} // namespace bsgalone::core
