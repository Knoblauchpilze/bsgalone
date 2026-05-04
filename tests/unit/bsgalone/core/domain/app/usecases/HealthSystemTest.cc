
#include "HealthSystem.hh"
#include "EntityRegistry.hh"
#include "HealthComponent.hh"
#include <gtest/gtest.h>

namespace bsgalone::core {

TEST(Unit_Bsgalone_Core_Domain_App_Usecases_HealthSystem, ThrowsWhenEntityRegistryIsNull)
{
  EXPECT_THROW([]() { HealthSystem(nullptr); }(), std::invalid_argument);
}

TEST(Unit_Bsgalone_Core_Domain_App_Usecases_HealthSystem, DoesNotUpdateDeadEntity)
{
  auto registry       = std::make_shared<EntityRegistry>();
  const auto entityId = registry->createEntity();
  registry->addComponent(entityId, HealthComponent{{.value = -1.0f}});

  HealthSystem usecase(registry);

  chrono::TickData data{.tick    = chrono::Tick::fromInt(1),
                        .elapsed = chrono::TickDuration::fromInt(1)};
  usecase.updateHealth(data);

  auto count = 0;
  registry->apply<const HealthComponent>([&count](const HealthComponent &component) {
    EXPECT_EQ(-1.0f, component.value);
    ++count;
  });
  EXPECT_EQ(1, count);
}

TEST(Unit_Bsgalone_Core_Domain_App_Usecases_HealthSystem, RegeneratesHealthForLivingEntity)
{
  auto registry       = std::make_shared<EntityRegistry>();
  const auto entityId = registry->createEntity();
  registry->addComponent(entityId,
                         HealthComponent{{.value = 1.0f, .max = 2.0f, .regenPerTick = 0.5f}});

  HealthSystem usecase(registry);

  chrono::TickData data{.tick    = chrono::Tick::fromInt(1),
                        .elapsed = chrono::TickDuration::fromInt(1)};
  usecase.updateHealth(data);

  auto count = 0;
  registry->apply<const HealthComponent>([&count](const HealthComponent &component) {
    EXPECT_EQ(1.5f, component.value);
    ++count;
  });
  EXPECT_EQ(1, count);
}

TEST(Unit_Bsgalone_Core_Domain_App_Usecases_HealthSystem, ClampsHealthToMaxValue)
{
  auto registry       = std::make_shared<EntityRegistry>();
  const auto entityId = registry->createEntity();
  registry->addComponent(entityId,
                         HealthComponent{{.value = 1.0f, .max = 2.0f, .regenPerTick = 2.0f}});

  HealthSystem usecase(registry);

  chrono::TickData data{.tick    = chrono::Tick::fromInt(1),
                        .elapsed = chrono::TickDuration::fromInt(1)};
  usecase.updateHealth(data);

  auto count = 0;
  registry->apply<const HealthComponent>([&count](const HealthComponent &component) {
    EXPECT_EQ(component.max, component.value);
    ++count;
  });
  EXPECT_EQ(1, count);
}

} // namespace bsgalone::core
