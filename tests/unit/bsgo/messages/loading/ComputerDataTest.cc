
#include "Common.hh"
#include "DataSerialization.hh"
#include <gtest/gtest.h>

using namespace ::testing;

namespace bsgo {
namespace {
inline bool serializeAndDeserializeData(const ComputerData &value, ComputerData &output)
{
  std::ostringstream out{};
  serializeComputerData(out, value);
  std::istringstream in(out.str());
  return deserializeComputerData(in, output);
}
} // namespace

TEST(Unit_Bsgo_Serialization_ComputerData, EqualWhenDbIdIsEqual)
{
  ComputerData data1{.dbId = Uuid{1234}, .offensive = true, .range = 123.456f};

  ComputerData data2{
    .dbId       = Uuid{1234},
    .reloadTime = TickDuration::fromInt(158),
    .duration   = TickDuration(789.357f),
  };

  EXPECT_TRUE(data1 == data2);
}

TEST(Unit_Bsgo_Serialization_ComputerData, DifferentWhenDbIdIsDifferent)
{
  ComputerData data1{.dbId      = Uuid{1234},
                     .powerCost = 100.0f,
                     .range     = 1.025f,
                     .price     = {{Uuid{26}, 401298}}};

  ComputerData data2 = data1;
  data2.dbId         = Uuid{5678};

  EXPECT_FALSE(data1 == data2);
}

TEST(Unit_Bsgo_Serialization_ComputerData, Basic)
{
  ComputerData input{.dbId           = Uuid{1234},
                     .name           = "eniac",
                     .range          = 98765.1234f,
                     .damageModifier = 2.10987f,
                     .price          = {{Uuid{17}, 9803}, {Uuid{3274}, 41097}}};

  ComputerData output{.dbId       = Uuid{14},
                      .powerCost  = 26.57f,
                      .reloadTime = TickDuration::fromInt(1234)};

  EXPECT_TRUE(serializeAndDeserializeData(input, output));

  assertComputerDataAreEqual(output, input);
}

TEST(Unit_Bsgo_Serialization_ComputerData, AllowedTargets)
{
  ComputerData input{.dbId = Uuid{1234}, .range = 98765.1234f, .damageModifier = 2.10987f};
  input.allowedTargets = std::unordered_set<EntityKind>{EntityKind::SHIP, EntityKind::ASTEROID};

  ComputerData output{.dbId       = Uuid{14},
                      .name       = "beefy computer",
                      .reloadTime = TickDuration(1234.0f)};

  EXPECT_TRUE(serializeAndDeserializeData(input, output));

  assertComputerDataAreEqual(output, input);
}

TEST(Unit_Bsgo_Serialization_ComputerData, NonEmptyAllowedTargetsInDestination)
{
  ComputerData input{.dbId           = Uuid{1234},
                     .name           = "a computer",
                     .range          = 98765.1234f,
                     .damageModifier = 2.10987f};

  ComputerData output{.dbId       = Uuid{14},
                      .name       = "another computer",
                      .reloadTime = TickDuration(1234.56008f),
                      .price      = {{Uuid{17}, 980}, {Uuid{3274}, 41}}};
  output.allowedTargets = std::unordered_set<EntityKind>{EntityKind::OUTPOST};

  EXPECT_TRUE(serializeAndDeserializeData(input, output));

  assertComputerDataAreEqual(output, input);
}

TEST(Unit_Bsgo_Serialization_ComputerData, NonEmptyDamageModifierInDestination)
{
  ComputerData input{.dbId = Uuid{1234}, .price = {{Uuid{17}, 982}, {Uuid{3274}, 417}}};

  ComputerData output{.dbId = Uuid{14}, .damageModifier = 2.10987f, .price = {{Uuid{89}, 3271}}};

  EXPECT_TRUE(serializeAndDeserializeData(input, output));

  assertComputerDataAreEqual(output, input);
}

} // namespace bsgo
