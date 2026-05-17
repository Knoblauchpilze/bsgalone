
#pragma once

#include "EntityRegistry.hh"
#include <gtest/gtest.h>

namespace test {

class EntityRegistryFixture : public ::testing::Test
{
  public:
  EntityRegistryFixture();
  ~EntityRegistryFixture() override = default;

  protected:
  int m_port{};

  void SetUp() override;
  void TearDown() override;

  auto entityRegistry() const -> bsgalone::core::EntityRegistryShPtr;

  private:
  bsgalone::core::EntityRegistryShPtr m_entityRegistry{};
};

} // namespace test
