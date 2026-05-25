
#pragma once

#include "Asteroid.hh"
#include "EntityRegistry.hh"
#include <gtest/gtest.h>

namespace test {

class EntityRegistryFixture : public ::testing::Test
{
  public:
  EntityRegistryFixture();
  ~EntityRegistryFixture() override = default;

  protected:
  void SetUp() override;
  void TearDown() override;

  auto entityRegistry() const -> bsgalone::core::EntityRegistryShPtr;

  void registerAsteroid(const bsgalone::core::Asteroid &asteroid);

  /// @brief - Verifies that the registry attached to this fixture does not contain
  /// any entity. The process is as described below:
  ///   - iterate over all entities with a DbComponent
  ///   - verify that no entity were found
  /// This approach works **as long as all entities have a DbComponent**.
  void assertRegistryEmpty();

  private:
  bsgalone::core::EntityRegistryShPtr m_entityRegistry{};
};

} // namespace test
