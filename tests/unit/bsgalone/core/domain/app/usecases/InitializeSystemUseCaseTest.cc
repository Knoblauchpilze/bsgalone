
#include "InitializeSystemUseCase.hh"
#include "DbComponent.hh"
#include "EntityRegistry.hh"
#include "HealthComponent.hh"
#include "MockAsteroidRepository.hh"
#include <gtest/gtest.h>

using namespace test;
using namespace ::testing;

namespace bsgalone::core {
namespace {
class Unit_Bsgalone_Core_Domain_App_Usecases_InitializeSystemUseCase : public Test
{
  protected:
  void SetUp() override
  {
    mockAsteroidRepo = std::make_shared<StrictMock<MockAsteroidRepository>>();
    entityRegistry   = std::make_shared<EntityRegistry>();

    usecase = std::make_unique<InitializeSystemUseCase>(mockAsteroidRepo, entityRegistry);
  }

  void TearDown() override
  {
    // Intentionally empty
  }

  std::shared_ptr<MockAsteroidRepository> mockAsteroidRepo{};
  std::shared_ptr<EntityRegistry> entityRegistry{};

  std::unique_ptr<InitializeSystemUseCase> usecase{};
};

auto generateAsteroid(const float seed, const Uuid systemDbId) -> Asteroid
{
  return Asteroid{
    .dbId       = Uuid{},
    .systemDbId = systemDbId,
    .position   = Eigen::Vector3f(1.0f * seed, -3.0f * seed, 9.5f * seed),
    .radius     = 1.78f * seed,
    .health     = 17.8f * seed,
    .maxHealth  = 58.2f * seed,
  };
}
} // namespace

TEST_F(Unit_Bsgalone_Core_Domain_App_Usecases_InitializeSystemUseCase,
       ThrowsWhenAsteroidRepositoryIsNull)
{
  EXPECT_THROW([this]() { InitializeSystemUseCase(nullptr, entityRegistry); }(),
               std::invalid_argument);
}

TEST_F(Unit_Bsgalone_Core_Domain_App_Usecases_InitializeSystemUseCase,
       ThrowsWhenEntityRegistryIsNull)
{
  EXPECT_THROW([this]() { InitializeSystemUseCase(mockAsteroidRepo, nullptr); }(),
               std::invalid_argument);
}

TEST_F(Unit_Bsgalone_Core_Domain_App_Usecases_InitializeSystemUseCase,
       DoesNotRegisterAsteroidWhenNoneExist)
{
  Uuid systemDbId{};
  EXPECT_CALL(*mockAsteroidRepo, findAllBySystem(systemDbId))
    .Times(1)
    .WillOnce(Return(std::vector<Asteroid>{}));

  usecase->initializeSystem(systemDbId);
}

TEST_F(Unit_Bsgalone_Core_Domain_App_Usecases_InitializeSystemUseCase,
       RegistersDbComponentsForAllAsteroids)
{
  Uuid systemDbId{};

  const auto asteroid1 = generateAsteroid(1.0f, systemDbId);
  const auto asteroid2 = generateAsteroid(2.0f, systemDbId);

  std::vector<Asteroid> asteroids{asteroid1, asteroid2};
  EXPECT_CALL(*mockAsteroidRepo, findAllBySystem(systemDbId)).Times(1).WillOnce(Return(asteroids));

  usecase->initializeSystem(systemDbId);

  std::unordered_set<Uuid> actualIds{};
  entityRegistry->apply<DbComponent>(
    [&actualIds](const DbComponent &component) { actualIds.emplace(component.dbId); });

  const std::unordered_set<Uuid> expectedIds{asteroid1.dbId, asteroid2.dbId};
  EXPECT_EQ(expectedIds, actualIds);
}

TEST_F(Unit_Bsgalone_Core_Domain_App_Usecases_InitializeSystemUseCase,
       RegistersHealthComponentsForAllAsteroids)
{
  Uuid systemDbId{};
  const auto asteroid1 = generateAsteroid(1.0f, systemDbId);
  const auto asteroid2 = generateAsteroid(2.0f, systemDbId);

  std::vector<Asteroid> asteroids{asteroid1, asteroid2};
  EXPECT_CALL(*mockAsteroidRepo, findAllBySystem(systemDbId)).Times(1).WillOnce(Return(asteroids));

  usecase->initializeSystem(systemDbId);

  struct HealthData
  {
    float min{0.0f};
    float value{0.0f};
    float max{0.0f};
    float regen{0.0f};

    bool operator==(const HealthData &rhs) const = default;
  };

  std::vector<HealthData> actualHealths{};
  entityRegistry->apply<HealthComponent>([&actualHealths](const HealthComponent &component) {
    actualHealths.emplace_back(
      HealthData{.min = component.min, .value = component.value, .max = component.max});
  });

  const std::vector<HealthData> expectedHealths{
    {.min = 0.0f, .value = asteroid2.health, .max = asteroid2.maxHealth, .regen = 0.0f},
    {.min = 0.0f, .value = asteroid1.health, .max = asteroid1.maxHealth, .regen = 0.0f},
  };
  EXPECT_EQ(expectedHealths, actualHealths);
}

} // namespace bsgalone::core
