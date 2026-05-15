
#include "InitializeSystemUseCase.hh"
#include "MockAsteroidRepository.hh"
#include "MockEntityManager.hh"
#include "TestDataFactory.hh"
#include <gtest/gtest.h>

using namespace test;
using namespace ::testing;

namespace bsgalone::server {
namespace {
class Unit_Bsgalone_Server_Domain_App_Usecases_InitializeSystemUseCase : public Test
{
  protected:
  void SetUp() override
  {
    mockAsteroidRepo  = std::make_shared<StrictMock<MockAsteroidRepository>>();
    mockEntityManager = std::make_shared<StrictMock<MockEntityManager>>();

    usecase = std::make_unique<InitializeSystemUseCase>(mockAsteroidRepo, mockEntityManager);
  }

  void TearDown() override
  {
    // Intentionally empty
  }

  std::shared_ptr<MockAsteroidRepository> mockAsteroidRepo{};
  std::shared_ptr<MockEntityManager> mockEntityManager{};

  std::unique_ptr<InitializeSystemUseCase> usecase{};
};
} // namespace

TEST_F(Unit_Bsgalone_Server_Domain_App_Usecases_InitializeSystemUseCase,
       ThrowsWhenAsteroidRepositoryIsNull)
{
  EXPECT_THROW([this]() { InitializeSystemUseCase(nullptr, mockEntityManager); }(),
               std::invalid_argument);
}

TEST_F(Unit_Bsgalone_Server_Domain_App_Usecases_InitializeSystemUseCase,
       ThrowsWhenEntityRepositoryIsNull)
{
  EXPECT_THROW([this]() { InitializeSystemUseCase(mockAsteroidRepo, nullptr); }(),
               std::invalid_argument);
}

TEST_F(Unit_Bsgalone_Server_Domain_App_Usecases_InitializeSystemUseCase,
       DoesNotCreateEntityWhenNoAsteroidExist)
{
  core::Uuid systemDbId{};
  EXPECT_CALL(*mockAsteroidRepo, findAllBySystem(systemDbId))
    .Times(1)
    .WillOnce(Return(std::vector<core::Asteroid>{}));

  usecase->initializeSystem(systemDbId);
}

TEST_F(Unit_Bsgalone_Server_Domain_App_Usecases_InitializeSystemUseCase,
       CreatesEntityForEachAsteroid)
{
  core::Uuid systemDbId{};

  const auto asteroid1 = core::Asteroid{
    .dbId      = core::Uuid{},
    .position  = Eigen::Vector3f(-2.0f, 4.0f, -3.85f),
    .radius    = 0.58f,
    .health    = 45.25f,
    .maxHealth = 62.21f,
  };
  const auto asteroid2 = core::Asteroid{
    .dbId      = core::Uuid{},
    .position  = Eigen::Vector3f(1.0f, -3.0f, 9.5f),
    .radius    = 1.78f,
    .health    = 17.8f,
    .maxHealth = 58.2f,
  };

  EXPECT_CALL(*mockAsteroidRepo, findAllBySystem(systemDbId))
    .Times(1)
    .WillOnce(Return(std::vector<core::Asteroid>{asteroid1, asteroid2}));

  core::Asteroid captured1{};
  core::Asteroid captured2{};
  EXPECT_CALL(*mockEntityManager, createAsteroid(_))
    .Times(2)
    .WillOnce(Invoke([&captured1](const core::Asteroid &asteroid) {
      captured1 = asteroid;
      return core::Uuid{};
    }))
    .WillOnce(Invoke([&captured2](const core::Asteroid &asteroid) {
      captured2 = asteroid;
      return core::Uuid{};
    }));

  usecase->initializeSystem(systemDbId);

  EXPECT_EQ(asteroid1, captured1);
  EXPECT_EQ(asteroid2, captured2);
}

} // namespace bsgalone::server
