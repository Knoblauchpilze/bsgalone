
#include "CreateAsteroidUseCase.hh"
#include "DbComponent.hh"
#include "EntityRegistry.hh"
#include "HealthComponent.hh"
#include <gtest/gtest.h>

using namespace ::testing;

namespace bsgalone::core {
namespace {
class Unit_Bsgalone_Core_Domain_App_Usecases_CreateAsteroidUseCase : public Test
{
  protected:
  void SetUp() override
  {
    entityRegistry = std::make_shared<EntityRegistry>();
    usecase        = std::make_unique<CreateAsteroidUseCase>(entityRegistry);
  }

  void TearDown() override
  {
    // Intentionally empty
  }

  std::shared_ptr<EntityRegistry> entityRegistry{};

  std::unique_ptr<CreateAsteroidUseCase> usecase{};
};

auto generateAsteroid() -> Asteroid
{
  return Asteroid{
    .dbId       = Uuid{},
    .systemDbId = Uuid{},
    .position   = Eigen::Vector3f(1.0f, -3.0f, 9.5f),
    .radius     = 1.78f,
    .health     = 17.8f,
    .maxHealth  = 58.2f,
  };
}
} // namespace

TEST_F(Unit_Bsgalone_Core_Domain_App_Usecases_CreateAsteroidUseCase, ThrowsWhenEntityRegistryIsNull)
{
  EXPECT_THROW([this]() { CreateAsteroidUseCase(nullptr); }(), std::invalid_argument);
}

TEST_F(Unit_Bsgalone_Core_Domain_App_Usecases_CreateAsteroidUseCase, RegistersDbComponent)
{
  const auto asteroid = generateAsteroid();

  const auto expectedEntityId = usecase->create(asteroid);

  int counter{0};
  entityRegistry->applyWithId<DbComponent>(
    [&counter, &asteroid, &expectedEntityId](const Uuid entityId, const DbComponent &component) {
      EXPECT_EQ(asteroid.dbId, component.dbId);
      EXPECT_EQ(expectedEntityId, entityId);

      ++counter;
    });
  EXPECT_EQ(1, counter);
}

TEST_F(Unit_Bsgalone_Core_Domain_App_Usecases_CreateAsteroidUseCase, RegistersHealthComponent)
{
  const auto asteroid = generateAsteroid();

  const auto expectedEntityId = usecase->create(asteroid);

  int counter{0};
  entityRegistry->applyWithId<HealthComponent>(
    [&counter, &asteroid, &expectedEntityId](const Uuid entityId, const HealthComponent &component) {
      EXPECT_EQ(0.0f, component.min);
      EXPECT_EQ(asteroid.health, component.value);
      EXPECT_EQ(asteroid.maxHealth, component.max);
      EXPECT_EQ(0.0f, component.regenPerTick);
      EXPECT_EQ(expectedEntityId, entityId);

      ++counter;
    });
  EXPECT_EQ(1, counter);
}

} // namespace bsgalone::core
