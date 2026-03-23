
#include "PlayerComputerData.hh"
#include "Common.hh"
#include "Comparison.hh"
#include "SerializationHelper.hh"
#include <gtest/gtest.h>

using namespace test;
using namespace ::testing;

namespace bsgalone::core {

TEST(Unit_Bsgalone_Core_Messages_PlayerComputerData, EqualWhenDbIdIsEqual)
{
  PlayerComputerData data1{.dbId         = Uuid{1234},
                           .computerDbId = Uuid{41},
                           .offensive    = true,
                           .range        = 123.456f};

  PlayerComputerData data2{
    .dbId       = Uuid{1234},
    .level      = 5,
    .reloadTime = chrono::TickDuration::fromInt(158),
    .duration   = chrono::TickDuration(789.2357f),
  };

  EXPECT_TRUE(data1 == data2);
}

TEST(Unit_Bsgalone_Core_Messages_PlayerComputerData, DifferentWhenDbIdIsDifferent)
{
  PlayerComputerData data1{.dbId         = Uuid{1234},
                           .computerDbId = Uuid{67},
                           .powerCost    = 100.0f,
                           .range        = 1.025f};

  PlayerComputerData data2 = data1;
  data2.dbId               = Uuid{5678};

  EXPECT_FALSE(data1 == data2);
}

TEST(Unit_Bsgalone_Core_Messages_PlayerComputerData, Basic)
{
  PlayerComputerData input{.dbId           = Uuid{1234},
                           .name           = "eniac",
                           .level          = 4,
                           .range          = 98765.1234f,
                           .damageModifier = 2.10987f};

  PlayerComputerData output{.dbId       = Uuid{14},
                            .level      = 12,
                            .reloadTime = chrono::TickDuration::fromInt(1234)};

  EXPECT_TRUE(test::serializeAndDeserialize(input, output));

  assertPlayerComputerDataAreEqual(output, input);
}

TEST(Unit_Bsgalone_Core_Messages_PlayerComputerData, AllowedTargets)
{
  PlayerComputerData input{.dbId           = Uuid{1234},
                           .level          = 4,
                           .range          = 98765.1234f,
                           .damageModifier = 2.10987f};
  input.allowedTargets = std::unordered_set<EntityKind>{EntityKind::SHIP, EntityKind::ASTEROID};

  PlayerComputerData output{.dbId       = Uuid{14},
                            .name       = "beefy computer",
                            .level      = 12,
                            .reloadTime = chrono::TickDuration(1234.9541f)};

  EXPECT_TRUE(test::serializeAndDeserialize(input, output));

  assertPlayerComputerDataAreEqual(output, input);
}

TEST(Unit_Bsgalone_Core_Messages_PlayerComputerData, NonEmptyAllowedTargetsInDestination)
{
  PlayerComputerData input{.dbId           = Uuid{1234},
                           .name           = "a computer",
                           .level          = 4,
                           .range          = 98765.1234f,
                           .damageModifier = 2.10987f};

  PlayerComputerData output{.dbId       = Uuid{14},
                            .name       = "another computer",
                            .level      = 12,
                            .reloadTime = chrono::TickDuration(1234.3247f)};
  output.allowedTargets = std::unordered_set<EntityKind>{EntityKind::OUTPOST};

  EXPECT_TRUE(test::serializeAndDeserialize(input, output));

  assertPlayerComputerDataAreEqual(output, input);
}

TEST(Unit_Bsgalone_Core_Messages_PlayerComputerData, NonEmptyDamageModifierInDestination)
{
  PlayerComputerData input{.dbId = Uuid{1234}};

  PlayerComputerData output{.dbId = Uuid{14}, .damageModifier = 2.10987f};

  EXPECT_TRUE(test::serializeAndDeserialize(input, output));

  assertPlayerComputerDataAreEqual(output, input);
}

} // namespace bsgalone::core
