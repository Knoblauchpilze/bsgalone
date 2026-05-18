
#include "FetchAsteroidUseCase.hh"
#include "EntityRegistry.hh"
#include "EntityRegistryFixture.hh"
#include "TestDataFactory.hh"
#include <gtest/gtest.h>

using namespace test;
using namespace ::testing;

namespace bsgalone::core {
namespace {
class Unit_Bsgalone_Core_Domain_App_Usecases_FetchAsteroidUseCase : public EntityRegistryFixture
{
  protected:
  void SetUp() override
  {
    this->EntityRegistryFixture::SetUp();
    usecase = std::make_unique<FetchAsteroidUseCase>(this->entityRegistry());
  }

  void TearDown() override
  {
    this->EntityRegistryFixture::TearDown();
  }

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
  this->registerAsteroid(asteroid1);
  const auto asteroid2 = generateAsteroid();
  this->registerAsteroid(asteroid2);

  const auto asteroids = usecase->getAllAsteroids();

  EXPECT_EQ(2u, asteroids.size());
  EXPECT_EQ(asteroid2, asteroids.at(0));
  EXPECT_EQ(asteroid1, asteroids.at(1));
}

TEST_F(Unit_Bsgalone_Core_Domain_App_Usecases_FetchAsteroidUseCase,
       ReturnsAsteroidWithLootWhenAvailable)
{
  auto asteroid = generateAsteroid(true);
  this->registerAsteroid(asteroid);

  const auto asteroids = usecase->getAllAsteroids();

  EXPECT_EQ(1u, asteroids.size());
  EXPECT_EQ(asteroid, asteroids.at(0));
}

} // namespace bsgalone::core
