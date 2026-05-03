
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

} // namespace bsgalone::core
