
#include "EcsCoordinator.hh"
#include "HealthSystem.hh"

namespace bsgalone::core {

EcsCoordinator::EcsCoordinator(EntityRegistryShPtr entityRegistry)
  : ForRunningSimulation()
{
  if (entityRegistry == nullptr)
  {
    throw std::invalid_argument("Expected non null entity registry");
  }

  initialize(std::move(entityRegistry));
}

void EcsCoordinator::update(const chrono::TickData &data)
{
  m_healthSystem->updateHealth(data);
}

void EcsCoordinator::clear()
{
  /// TODO: Handle the reset of the ECS
}

void EcsCoordinator::initialize(EntityRegistryShPtr entityRegistry)
{
  m_healthSystem = std::make_unique<HealthSystem>(entityRegistry);
}

} // namespace bsgalone::core
