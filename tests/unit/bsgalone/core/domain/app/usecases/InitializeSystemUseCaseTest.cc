
#include "InitializeSystemUseCase.hh"
#include "MockAsteroidRepository.hh"
#include "MockEntityRegistry.hh"
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
    mockAsteroidRepo   = std::make_shared<StrictMock<MockAsteroidRepository>>();
    mockEntityRegistry = std::make_shared<StrictMock<MockEntityRegistry>>();

    usecase = std::make_unique<InitializeSystemUseCase>(mockAsteroidRepo, mockEntityRegistry);
  }

  void TearDown() override
  {
    // Intentionally empty
  }

  std::shared_ptr<MockAsteroidRepository> mockAsteroidRepo{};
  std::shared_ptr<MockEntityRegistry> mockEntityRegistry{};

  std::unique_ptr<InitializeSystemUseCase> usecase{};
};
} // namespace

TEST_F(Unit_Bsgalone_Core_Domain_App_Usecases_InitializeSystemUseCase,
       ThrowsWhenAsteroidRepositoryIsNull)
{
  EXPECT_THROW([this]() { InitializeSystemUseCase(nullptr, mockEntityRegistry); }(),
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

  EXPECT_CALL(*mockEntityRegistry, registerAsteroid(_))
    .Times(2)
    .WillOnce(Invoke([&asteroidDbId1](const Asteroid &asteroid) {
      EXPECT_EQ(asteroidDbId1, asteroid.dbId);
      return EntityId{};
    }))
    .WillOnce(Invoke([&asteroidDbId2](const Asteroid &asteroid) {
      EXPECT_EQ(asteroidDbId2, asteroid.dbId);
      return EntityId{};
    }));

  usecase->initializeSystem(systemDbId);
}

} // namespace bsgalone::core
