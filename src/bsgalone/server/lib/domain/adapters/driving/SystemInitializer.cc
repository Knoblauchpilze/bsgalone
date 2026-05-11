
#include "SystemInitializer.hh"
#include "AsteroidRepository.hh"
#include "EcsCoordinator.hh"
#include "EntityManager.hh"
#include "InitializeSystemUseCase.hh"
#include "SystemProcessor.hh"

namespace bsgalone::server {

auto SystemInitializer::initializeSystem(const System &system) -> core::SystemProcessorShPtr
{
  auto manager     = std::make_unique<chrono::TimeManager>(system.currentTick, system.step);
  auto registry    = std::make_shared<core::EntityRegistry>();
  auto coordinator = std::make_unique<core::EcsCoordinator>(registry);
  auto processor   = std::make_shared<core::SystemProcessor>(system.name,
                                                           std::move(coordinator),
                                                           std::move(manager));

  registerAsteroids(system.dbId, registry);

  return processor;
}

void SystemInitializer::registerAsteroids(const core::Uuid &systemDbId,
                                          core::EntityRegistryShPtr registry)
{
  auto entityManager = std::make_shared<EntityManager>(registry);

  InitializeSystemUseCase usecase(m_repositories.asteroidRepository, entityManager);
  usecase.initializeSystem(systemDbId);
}

} // namespace bsgalone::server
