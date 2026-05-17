
#include "FetchAsteroidUseCase.hh"
#include "CircleBox.hh"
#include "DbComponent.hh"
#include "EntityRegistry.hh"
#include "HealthComponent.hh"
#include "ResourceComponent.hh"
#include "TestDataFactory.hh"
#include "TransformComponent.hh"
#include <gtest/gtest.h>

using namespace test;
using namespace ::testing;

namespace bsgalone::core {
namespace {
class Unit_Bsgalone_Core_Domain_App_Usecases_FetchAsteroidUseCase : public Test
{
  protected:
  void SetUp() override
  {
    entityRegistry = std::make_shared<EntityRegistry>();
    usecase        = std::make_unique<FetchAsteroidUseCase>(entityRegistry);
  }

  void TearDown() override
  {
    // Intentionally empty
  }

  // TODO: Use the EntityRegistryFixture
  void registerAsteroid(const Asteroid &asteroid)
  {
    const auto entityId = entityRegistry->createEntity();
    entityRegistry->addComponent(entityId, DbComponent{.dbId = asteroid.dbId});
    entityRegistry->addComponent(entityId,
                                 HealthComponent{{
                                   .value = asteroid.health,
                                   .max   = asteroid.maxHealth,
                                 }});
    auto box = std::make_unique<CircleBox>(asteroid.position, asteroid.radius);
    entityRegistry->addComponent(entityId,
                                 TransformComponent{.bbox = std::move(box), .heading = 0.0f});
    if (asteroid.loot)
    {
      entityRegistry->addComponent(entityId,
                                   ResourceComponent{.resource = asteroid.loot->resource,
                                                     .amount   = asteroid.loot->amount});
    }
  }

  std::shared_ptr<EntityRegistry> entityRegistry{};

  std::unique_ptr<FetchAsteroidUseCase> usecase{};
};
} // namespace

TEST_F(Unit_Bsgalone_Core_Domain_App_Usecases_FetchAsteroidUseCase, ThrowsWhenEntityRegistryIsNull)
{
  EXPECT_THROW([this]() { FetchAsteroidUseCase(nullptr); }(), std::invalid_argument);
}

TEST_F(Unit_Bsgalone_Core_Domain_App_Usecases_FetchAsteroidUseCase, ReturnsAsteroidWhenAvailable)
{
  const auto asteroid1 = generateAsteroid();
  registerAsteroid(asteroid1);
  const auto asteroid2 = generateAsteroid();
  registerAsteroid(asteroid2);

  const auto asteroids = usecase->getAllAsteroids();

  EXPECT_EQ(2u, asteroids.size());
  EXPECT_EQ(asteroid2, asteroids.at(0));
  EXPECT_EQ(asteroid1, asteroids.at(1));
}

TEST_F(Unit_Bsgalone_Core_Domain_App_Usecases_FetchAsteroidUseCase,
       ReturnsAsteroidWithLootWhenAvailable)
{
  auto asteroid = generateAsteroid(true);
  registerAsteroid(asteroid);

  const auto asteroids = usecase->getAllAsteroids();

  EXPECT_EQ(1u, asteroids.size());
  EXPECT_EQ(asteroid, asteroids.at(0));
}

} // namespace bsgalone::core
