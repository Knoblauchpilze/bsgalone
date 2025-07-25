
#include "Common.hh"
#include "DataSerialization.hh"
#include <gtest/gtest.h>

using namespace ::testing;

namespace bsgo {
namespace {
inline bool serializeAndDeserializeData(const PlayerComputerData &value, PlayerComputerData &output)
{
  std::ostringstream out{};
  serializePlayerComputerData(out, value);
  std::istringstream in(out.str());
  return deserializePlayerComputerData(in, output);
}
} // namespace

TEST(Unit_Bsgo_Serialization_PlayerComputerData, EqualWhenDbIdIsEqual)
{
  PlayerComputerData data1{.dbId         = Uuid{1234},
                           .computerDbId = Uuid{41},
                           .offensive    = true,
                           .range        = 123.456f};

  PlayerComputerData data2{
    .dbId       = Uuid{1234},
    .level      = 5,
    .reloadTime = core::toMilliseconds(158),
    .duration   = core::toMilliseconds(789),
  };

  EXPECT_TRUE(data1 == data2);
}

TEST(Unit_Bsgo_Serialization_PlayerComputerData, DifferentWhenDbIdIsDifferent)
{
  PlayerComputerData data1{.dbId         = Uuid{1234},
                           .computerDbId = Uuid{67},
                           .powerCost    = 100.0f,
                           .range        = 1.025f};

  PlayerComputerData data2 = data1;
  data2.dbId               = Uuid{5678};

  EXPECT_FALSE(data1 == data2);
}

TEST(Unit_Bsgo_Serialization_PlayerComputerData, Basic)
{
  PlayerComputerData input{.dbId           = Uuid{1234},
                           .level          = 4,
                           .range          = 98765.1234f,
                           .damageModifier = 2.10987f};

  PlayerComputerData output{.dbId = Uuid{14}, .level = 12, .reloadTime = core::toMilliseconds(1234)};

  EXPECT_TRUE(serializeAndDeserializeData(input, output));

  assertPlayerComputerDataAreEqual(output, input);
}

TEST(Unit_Bsgo_Serialization_PlayerComputerData, AllowedTargets)
{
  PlayerComputerData input{.dbId           = Uuid{1234},
                           .level          = 4,
                           .range          = 98765.1234f,
                           .damageModifier = 2.10987f};
  input.allowedTargets = std::unordered_set<EntityKind>{EntityKind::SHIP, EntityKind::ASTEROID};

  PlayerComputerData output{.dbId = Uuid{14}, .level = 12, .reloadTime = core::toMilliseconds(1234)};

  EXPECT_TRUE(serializeAndDeserializeData(input, output));

  assertPlayerComputerDataAreEqual(output, input);
}

TEST(Unit_Bsgo_Serialization_PlayerComputerData, NonEmptyAllowedTargetsInDestination)
{
  PlayerComputerData input{.dbId           = Uuid{1234},
                           .level          = 4,
                           .range          = 98765.1234f,
                           .damageModifier = 2.10987f};

  PlayerComputerData output{.dbId = Uuid{14}, .level = 12, .reloadTime = core::toMilliseconds(1234)};
  output.allowedTargets = std::unordered_set<EntityKind>{EntityKind::OUTPOST};

  EXPECT_TRUE(serializeAndDeserializeData(input, output));

  assertPlayerComputerDataAreEqual(output, input);
}

TEST(Unit_Bsgo_Serialization_PlayerComputerData, NonEmptyDamageModifierInDestination)
{
  PlayerComputerData input{.dbId = Uuid{1234}};

  PlayerComputerData output{.dbId = Uuid{14}, .damageModifier = 2.10987f};

  EXPECT_TRUE(serializeAndDeserializeData(input, output));

  assertPlayerComputerDataAreEqual(output, input);
}

} // namespace bsgo
