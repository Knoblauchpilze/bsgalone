
#include "EcsCoordinator.hh"
#include "HealthSystem.hh"

namespace bsgalone::core {

EcsCoordinator::EcsCoordinator(EntityRegistryShPtr entityRegistry)
  : IEcsCoordinator()
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

void EcsCoordinator::initialize(EntityRegistryShPtr entityRegistry)
{
  m_healthSystem = std::make_unique<HealthSystem>(entityRegistry);
}

} // namespace bsgalone::core
