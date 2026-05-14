
#include "CreateAsteroidUseCase.hh"
#include "CircleBox.hh"
#include "DbComponent.hh"
#include "HealthComponent.hh"
#include "TransformComponent.hh"

namespace bsgalone::core {

CreateAsteroidUseCase::CreateAsteroidUseCase(EntityRegistryShPtr entityRegistry)
  : m_entityRegistry(std::move(entityRegistry))
{
  if (m_entityRegistry == nullptr)
  {
    throw std::invalid_argument("Expected non null entity registry");
  }
}

auto CreateAsteroidUseCase::create(const Asteroid &asteroid) -> Uuid
{
  const auto entityId = m_entityRegistry->createEntity();

  m_entityRegistry->addComponent(entityId, DbComponent{.dbId = asteroid.dbId});
  // https://stackoverflow.com/questions/47333843/using-initializer-list-for-a-struct-with-inheritance
  m_entityRegistry->addComponent(entityId,
                                 HealthComponent{{
                                   .value = asteroid.health,
                                   .max   = asteroid.maxHealth,
                                 }});
  auto box = std::make_unique<CircleBox>(asteroid.position, asteroid.radius);
  m_entityRegistry->addComponent(entityId,
                                 TransformComponent{.bbox = std::move(box), .heading = 0.0f});

  return entityId;
}

} // namespace bsgalone::core
