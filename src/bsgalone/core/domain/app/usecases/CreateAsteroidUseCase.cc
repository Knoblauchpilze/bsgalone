
#include "CreateAsteroidUseCase.hh"
#include "DbComponent.hh"
#include "HealthComponent.hh"

namespace bsgalone::core {

CreateAsteroidUseCase::CreateAsteroidUseCase(EntityRegistryShPtr entityRegisry)
  : m_entityRegisry(std::move(entityRegisry))
{
  if (m_entityRegisry == nullptr)
  {
    throw std::invalid_argument("Expected non null entity registry");
  }
}

auto CreateAsteroidUseCase::create(const Asteroid &asteroid) -> Uuid
{
  const auto entityId = m_entityRegisry->createEntity();
  m_entityRegisry->addComponent(entityId, DbComponent{.dbId = asteroid.dbId});
  // https://stackoverflow.com/questions/47333843/using-initializer-list-for-a-struct-with-inheritance
  m_entityRegisry->addComponent(entityId,
                                HealthComponent{{
                                  .value = asteroid.health,
                                  .max   = asteroid.maxHealth,
                                }});

  return entityId;
}

} // namespace bsgalone::core
