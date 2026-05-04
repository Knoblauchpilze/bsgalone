
#include "HealthSystem.hh"

namespace bsgalone::core {

HealthSystem::HealthSystem(EntityRegistryShPtr entityRegistry)
  : ForManagingHealth()
  , m_entityRegistry(std::move(entityRegistry))
{
  if (m_entityRegistry == nullptr)
  {
    throw std::invalid_argument("Expected non null entity registry");
  }
}

void HealthSystem::updateHealth(const chrono::TickData &data)
{
  m_entityRegistry->apply<HealthComponent>(
    [this, &data](HealthComponent &component) { updateComponent(component, data); });
}

void HealthSystem::updateComponent(HealthComponent &component, const chrono::TickData &data)
{
  if (!component.isAlive())
  {
    return;
  }

  const auto updated = component.value + component.regenPerTick * data.elapsed;
  component.value    = std::clamp(updated, component.min, component.max);
}

} // namespace bsgalone::core
