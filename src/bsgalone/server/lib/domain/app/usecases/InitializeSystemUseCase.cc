
#include "InitializeSystemUseCase.hh"
#include "DbComponent.hh"
#include "HealthComponent.hh"

namespace bsgalone::server {

InitializeSystemUseCase::InitializeSystemUseCase(ForManagingAsteroidShPtr asteroidRepo,
                                                 ForManagingEntityShPtr entityRepo)
  : m_asteroidRepo(std::move(asteroidRepo))
  , m_entityRepo(std::move(entityRepo))
{
  if (m_asteroidRepo == nullptr)
  {
    throw std::invalid_argument("Expected non null asteroid repository");
  }
  if (m_entityRepo == nullptr)
  {
    throw std::invalid_argument("Expected non null entity repository");
  }
}

void InitializeSystemUseCase::initializeSystem(const core::Uuid systemDbId)
{
  initializeAsteroids(systemDbId);
}

void InitializeSystemUseCase::initializeAsteroids(const core::Uuid systemDbId)
{
  for (const auto &asteroid : m_asteroidRepo->findAllBySystem(systemDbId))
  {
    m_entityRepo->createAsteroid(asteroid);
  }
}

} // namespace bsgalone::server
