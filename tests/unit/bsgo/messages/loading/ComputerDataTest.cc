
#include "ComputerData.hh"
#include <gtest/gtest.h>

using namespace ::testing;

namespace bsgo {
namespace {
inline bool serializeAndDeserializeMessage(const ComputerData &value, ComputerData &output)
{
  std::ostringstream out{};
  serializeComputerData(out, value);
  std::istringstream in(out.str());
  return deserializeComputerData(in, output);
}

auto assertDataAreEqual(const ComputerData &actual, const ComputerData &expected)
{
  EXPECT_EQ(actual.dbId, expected.dbId);
  EXPECT_EQ(actual.computerDbId, expected.computerDbId);
  EXPECT_EQ(actual.level, expected.level);
  EXPECT_EQ(actual.offensive, expected.offensive);
  EXPECT_EQ(actual.powerCost, expected.powerCost);
  EXPECT_EQ(actual.range, expected.range);
  EXPECT_EQ(actual.reloadTime, expected.reloadTime);
  EXPECT_EQ(actual.duration, expected.duration);
  EXPECT_EQ(actual.allowedTargets, expected.allowedTargets);
  EXPECT_EQ(actual.damageModifier, expected.damageModifier);
}
} // namespace

TEST(Unit_Bsgo_Serialization_ComputerData, EqualWhenDbIdIsEqual)
{
  ComputerData data1{.dbId         = Uuid{1234},
                     .computerDbId = Uuid{41},
                     .offensive    = true,
                     .range        = 123.456f};

  ComputerData data2{
    .dbId       = Uuid{1234},
    .level      = 5,
    .reloadTime = core::toMilliseconds(158),
    .duration   = core::toMilliseconds(789),
  };

  EXPECT_TRUE(data1 == data2);
}

TEST(Unit_Bsgo_Serialization_ComputerData, DifferentWhenDbIdIsDifferent)
{
  ComputerData data1{.dbId         = Uuid{1234},
                     .computerDbId = Uuid{67},
                     .powerCost    = 100.0f,
                     .range        = 1.025f};

  ComputerData data2 = data1;
  data2.dbId         = Uuid{5678};

  EXPECT_FALSE(data1 == data2);
}

TEST(Unit_Bsgo_Serialization_ComputerData, Basic)
{
  ComputerData input{.dbId           = Uuid{1234},
                     .level          = 4,
                     .range          = 98765.1234f,
                     .damageModifier = 2.10987f};

  ComputerData output{.dbId = Uuid{14}, .level = 12, .reloadTime = core::toMilliseconds(1234)};

  EXPECT_TRUE(serializeAndDeserializeMessage(input, output));

  assertDataAreEqual(output, input);
}

TEST(Unit_Bsgo_Serialization_ComputerData, AllowedTargets)
{
  ComputerData input{.dbId           = Uuid{1234},
                     .level          = 4,
                     .range          = 98765.1234f,
                     .damageModifier = 2.10987f};
  input.allowedTargets = std::unordered_set<EntityKind>{EntityKind::SHIP, EntityKind::ASTEROID};

  ComputerData output{.dbId = Uuid{14}, .level = 12, .reloadTime = core::toMilliseconds(1234)};

  EXPECT_TRUE(serializeAndDeserializeMessage(input, output));

  assertDataAreEqual(output, input);
}

TEST(Unit_Bsgo_Serialization_ComputerData, NonEmptyAllowedTargetsInDestination)
{
  ComputerData input{.dbId           = Uuid{1234},
                     .level          = 4,
                     .range          = 98765.1234f,
                     .damageModifier = 2.10987f};

  ComputerData output{.dbId = Uuid{14}, .level = 12, .reloadTime = core::toMilliseconds(1234)};
  output.allowedTargets = std::unordered_set<EntityKind>{EntityKind::OUTPOST};

  EXPECT_TRUE(serializeAndDeserializeMessage(input, output));

  assertDataAreEqual(output, input);
}

TEST(Unit_Bsgo_Serialization_ComputerData, NonEmptyDamageModifierInDestination)
{
  ComputerData input{.dbId = Uuid{1234}};

  ComputerData output{.dbId = Uuid{14}, .damageModifier = 2.10987f};

  EXPECT_TRUE(serializeAndDeserializeMessage(input, output));

  assertDataAreEqual(output, input);
}

} // namespace bsgo
