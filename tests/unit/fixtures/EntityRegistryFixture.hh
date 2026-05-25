
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

  private:
  bsgalone::core::EntityRegistryShPtr m_entityRegistry{};
};

} // namespace test
