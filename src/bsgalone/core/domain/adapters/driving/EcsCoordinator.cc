
#include "EcsCoordinator.hh"
#include "HealthSystem.hh"

namespace bsgalone::core {

EcsCoordinator::EcsCoordinator(EntityRegistryShPtr entityRegistry)
  : ForRunningSimulation()
  , m_entityRegistry(std::move(entityRegistry))
{
  if (m_entityRegistry == nullptr)
  {
    throw std::invalid_argument("Expected non null entity registry");
  }

  initialize();
}

void EcsCoordinator::update(const chrono::TickData &data)
{
  m_healthSystem->updateHealth(data);
}

void EcsCoordinator::clear()
{
  m_entityRegistry->clear();
}

void EcsCoordinator::initialize()
{
  m_healthSystem = std::make_unique<HealthSystem>(m_entityRegistry);
}

} // namespace bsgalone::core
