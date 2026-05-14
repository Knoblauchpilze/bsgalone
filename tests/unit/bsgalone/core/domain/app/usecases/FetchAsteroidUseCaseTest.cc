
#include "FetchAsteroidUseCase.hh"
#include "CircleBox.hh"
#include "DbComponent.hh"
#include "EntityRegistry.hh"
#include "HealthComponent.hh"
#include "TransformComponent.hh"
#include <gtest/gtest.h>

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

  std::shared_ptr<EntityRegistry> entityRegistry{};

  std::unique_ptr<FetchAsteroidUseCase> usecase{};
};
} // namespace

TEST_F(Unit_Bsgalone_Core_Domain_App_Usecases_FetchAsteroidUseCase, ThrowsWhenEntityRegistryIsNull)
{
  EXPECT_THROW([this]() { FetchAsteroidUseCase(nullptr); }(), std::invalid_argument);
}

// TODO: Add more tests

} // namespace bsgalone::core
