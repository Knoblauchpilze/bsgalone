
#include "InitializeSystemUseCase.hh"
#include "DbComponent.hh"
#include "HealthComponent.hh"

namespace bsgalone::core {

InitializeSystemUseCase::InitializeSystemUseCase(ForManagingAsteroidShPtr asteroidRepo,
                                                 EntityRegistryShPtr entityRegisry)
  : m_asteroidRepo(std::move(asteroidRepo))
  , m_entityRegisry(std::move(entityRegisry))
{
  if (m_asteroidRepo == nullptr)
  {
    throw std::invalid_argument("Expected non null asteroid repository");
  }
  if (m_entityRegisry == nullptr)
  {
    throw std::invalid_argument("Expected non null entity registry");
  }
}

void InitializeSystemUseCase::initializeSystem(const Uuid systemDbId)
{
  initializeAsteroids(systemDbId);
}

void InitializeSystemUseCase::initializeAsteroids(const Uuid systemDbId)
{
  for (const auto &asteroid : m_asteroidRepo->findAllBySystem(systemDbId))
  {
    const auto uuid = m_entityRegisry->createEntity();
    m_entityRegisry->addComponent(uuid, DbComponent{.dbId = asteroid.dbId});
    // https://stackoverflow.com/questions/47333843/using-initializer-list-for-a-struct-with-inheritance
    m_entityRegisry->addComponent(uuid,
                                  HealthComponent{{
                                    .value = asteroid.health,
                                    .max   = asteroid.maxHealth,
                                  }});
  }
}

} // namespace bsgalone::core
