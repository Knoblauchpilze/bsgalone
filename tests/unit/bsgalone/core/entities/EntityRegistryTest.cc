
#include "EntityRegistry.hh"
#include "FactionComponent.hh"
#include <gmock/gmock-matchers.h>
#include <gtest/gtest.h>

using namespace ::testing;

namespace bsgalone::core {

TEST(Unit_Bsgalone_Core_Entities_EntityRegistry, ThrowsWhenAddingComponentToUnknownEntity)
{
  auto code = []() {
    EntityRegistry registry;
    FactionComponent component{.faction = Faction::COLONIAL};
    registry.addComponent(Uuid{17}, std::move(component));
  };
  EXPECT_THAT(code, ThrowsMessage<std::invalid_argument>("No such entity 17"));
}

TEST(Unit_Bsgalone_Core_Entities_EntityRegistry, RegistersComponentForEntity)
{
  EntityRegistry registry;

  const auto entityId = registry.createEntity();

  FactionComponent component{.faction = Faction::CYLON};
  registry.addComponent(entityId, std::move(component));

  int counter = 0;
  registry.apply<FactionComponent>(
    [&counter](const FactionComponent & /*component*/) { ++counter; });

  EXPECT_EQ(1, counter);
}

} // namespace bsgalone::core
