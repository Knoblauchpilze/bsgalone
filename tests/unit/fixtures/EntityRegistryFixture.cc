
#include "EntityRegistryFixture.hh"
#include "CircleBox.hh"
#include "DbComponent.hh"
#include "EntityRegistryFixture.hh"
#include "HealthComponent.hh"
#include "ResourceComponent.hh"
#include "TransformComponent.hh"

namespace test {

EntityRegistryFixture::EntityRegistryFixture()
  : m_entityRegistry(std::make_shared<bsgalone::core::EntityRegistry>())
{}

void EntityRegistryFixture::SetUp() {}

void EntityRegistryFixture::TearDown() {}

auto EntityRegistryFixture::entityRegistry() const -> bsgalone::core::EntityRegistryShPtr
{
  return m_entityRegistry;
}

void EntityRegistryFixture::registerAsteroid(const bsgalone::core::Asteroid &asteroid)
{
  const auto entityId = m_entityRegistry->createEntity();
  m_entityRegistry->addComponent(entityId, bsgalone::core::DbComponent{.dbId = asteroid.dbId});
  m_entityRegistry->addComponent(entityId,
                                 bsgalone::core::HealthComponent{{
                                   .value = asteroid.health,
                                   .max   = asteroid.maxHealth,
                                 }});
  auto box = std::make_unique<bsgalone::core::CircleBox>(asteroid.position, asteroid.radius);
  m_entityRegistry->addComponent(entityId,
                                 bsgalone::core::TransformComponent{.bbox    = std::move(box),
                                                                    .heading = 0.0f});
  if (asteroid.loot)
  {
    m_entityRegistry
      ->addComponent(entityId,
                     bsgalone::core::ResourceComponent{.resource = asteroid.loot->resource,
                                                       .amount   = asteroid.loot->amount});
  }
}

void EntityRegistryFixture::assertRegistryEmpty()
{
  int counter{0};
  m_entityRegistry->apply<bsgalone::core::DbComponent>(
    [&counter](const bsgalone::core::DbComponent & /*component*/) { ++counter; });
  EXPECT_EQ(0, counter) << "Expected no entity in registry";
}

} // namespace test
