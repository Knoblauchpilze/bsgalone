
#include "InitializeSystemUseCase.hh"
#include "DbComponent.hh"
#include "EntityRegistry.hh"
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
       RegistersAllAsteroidsAsEntities)
{
  Uuid systemDbId{};

  Uuid asteroidDbId1{};
  Uuid asteroidDbId2{};

  std::vector<Asteroid> asteroids{
    Asteroid{.dbId = asteroidDbId1, .systemDbId = systemDbId},
    Asteroid{.dbId = asteroidDbId2, .systemDbId = systemDbId},
  };
  EXPECT_CALL(*mockAsteroidRepo, findAllBySystem(systemDbId)).Times(1).WillOnce(Return(asteroids));

  usecase->initializeSystem(systemDbId);

  std::unordered_set<Uuid> actualIds{};
  entityRegistry->apply<DbComponent>(
    [&actualIds](const DbComponent &component) { actualIds.emplace(component.dbId); });

  const std::unordered_set<Uuid> expectedIds{asteroidDbId1, asteroidDbId2};
  EXPECT_EQ(expectedIds, actualIds);
}

} // namespace bsgalone::core
