
#include "EntityManager.hh"
#include "CreateAsteroidUseCase.hh"

namespace bsgalone::server {

EntityManager::EntityManager(core::EntityRegistryShPtr registry)
{
  if (registry == nullptr)
  {
    throw std::invalid_argument("Expected non null registry");
  }

  initialize(std::move(registry));
}

void EntityManager::createAsteroid(const core::Asteroid &asteroid)
{
  m_asteroidCreator->create(asteroid);
}

void EntityManager::initialize(core::EntityRegistryShPtr registry)
{
  m_asteroidCreator = std::make_unique<core::CreateAsteroidUseCase>(registry);
}

} // namespace bsgalone::server
