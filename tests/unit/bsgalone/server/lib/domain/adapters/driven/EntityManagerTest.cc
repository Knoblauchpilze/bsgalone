
#include "EntityManager.hh"
#include "DbComponent.hh"
#include <gtest/gtest.h>

using namespace ::testing;

namespace bsgalone::server {

TEST(Unit_Bsgalone_Server_Domain_Adapters_Driven_EntityManager, ThrowsWhenEntityRegistryIsNull)
{
  EXPECT_THROW([]() { EntityManager(nullptr); }(), std::invalid_argument);
}

TEST(Unit_Bsgalone_Server_Domain_Adapters_Driven_EntityManager, CreatesEntityForAsteroid)
{
  auto registry = std::make_shared<core::EntityRegistry>();
  EntityManager manager(registry);

  core::Asteroid asteroid{
    .dbId      = core::Uuid{},
    .position  = Eigen::Vector3f(1.2f, 3.4f, 5.6f),
    .radius    = 7.87f,
    .health    = 9.56f,
    .maxHealth = 10.0f,
  };
  auto expectedEntityId = manager.createAsteroid(asteroid);

  registry->applyWithId<const core::DbComponent>(
    [&expectedEntityId, &asteroid](const core::Uuid entityId, const core::DbComponent &component) {
      EXPECT_EQ(expectedEntityId, entityId);
      EXPECT_EQ(asteroid.dbId, component.dbId);
    });
}

} // namespace bsgalone::server
