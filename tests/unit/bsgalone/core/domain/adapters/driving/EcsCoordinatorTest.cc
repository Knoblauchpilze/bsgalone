
#include "EcsCoordinator.hh"
#include "HealthComponent.hh"
#include <gtest/gtest.h>

using namespace ::testing;

namespace bsgalone::core {

TEST(Unit_Bsgalone_Core_Runtime_EcsCoordinator, ThrowsWhenEntityRegistryIsNull)
{
  EXPECT_THROW([]() { EcsCoordinator(nullptr); }(), std::invalid_argument);
}

TEST(Unit_Bsgalone_Core_Runtime_EcsCoordinator, UpdatesHealthOfEntity)
{
  auto registry       = std::make_shared<EntityRegistry>();
  const auto entityId = registry->createEntity();
  registry->addComponent(entityId,
                         HealthComponent{{.value = 0.5f, .max = 1.5f, .regenPerTick = 0.5f}});

  EcsCoordinator coordinator(registry);

  chrono::TickData data{.tick    = chrono::Tick::fromInt(1),
                        .elapsed = chrono::TickDuration::fromInt(1)};
  coordinator.update(data);

  auto count = 0;
  registry->apply<const HealthComponent>([&count](const HealthComponent &component) {
    EXPECT_EQ(1.0f, component.value);
    ++count;
  });
  EXPECT_EQ(1, count);
}

TEST(Unit_Bsgalone_Core_Runtime_EcsCoordinator, ClearsAllEntitiesOnReset)
{
  auto registry       = std::make_shared<EntityRegistry>();
  const auto entityId = registry->createEntity();
  registry->addComponent(entityId,
                         HealthComponent{{.value = 0.5f, .max = 1.5f, .regenPerTick = 0.5f}});

  EcsCoordinator coordinator(registry);

  coordinator.clear();

  auto count = 0;
  registry->apply<const HealthComponent>(
    [&count](const HealthComponent & /*component*/) { ++count; });
  EXPECT_EQ(0, count);
}

} // namespace bsgalone::core
