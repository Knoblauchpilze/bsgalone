
#pragma once

#include "EntityRegistry.hh"
#include "ForManagingHealth.hh"
#include "HealthComponent.hh"

namespace bsgalone::core {

class HealthSystem : public ForManagingHealth
{
  public:
  HealthSystem(EntityRegistryShPtr entityRegistry);
  ~HealthSystem() override = default;

  void updateHealth(const chrono::TickData &data) override;

  private:
  EntityRegistryShPtr m_entityRegistry{};

  void updateComponent(HealthComponent &component, const chrono::TickData &data);
};

} // namespace bsgalone::core
