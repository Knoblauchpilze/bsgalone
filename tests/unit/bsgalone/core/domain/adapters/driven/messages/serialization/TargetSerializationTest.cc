
#include "TargetSerialization.hh"
#include "Common.hh"
#include "Comparison.hh"
#include <gtest/gtest.h>

using namespace test;
using namespace ::testing;

namespace bsgalone::core {

TEST(Unit_Bsgalone_Core_Domain_Adapters_Messages_Serialization_TargetSerialization,
     SerializationDeserialization)
{
  Target input{.sourceDbId = Uuid{1234},
               .sourceKind = EntityKind::PLAYER,
               .targetDbId = Uuid{14},
               .targetKind = EntityKind::SHIP};

  Target output{.sourceDbId = Uuid{17640},
                .sourceKind = EntityKind::OUTPOST,
                .targetDbId = Uuid{5874},
                .targetKind = EntityKind::ASTEROID};

  EXPECT_TRUE(serializeAndDeserialize(input, output));

  assertTargetAreEqual(output, input);
}

TEST(Unit_Bsgalone_Core_Domain_Adapters_Messages_Serialization_TargetSerialization, OverridesTarget)
{
  Target input{.sourceDbId = Uuid{1234}, .sourceKind = EntityKind::PLAYER};

  Target output{.sourceDbId = Uuid{17640},
                .sourceKind = EntityKind::OUTPOST,
                .targetDbId = Uuid{5874},
                .targetKind = EntityKind::ASTEROID};

  EXPECT_TRUE(serializeAndDeserialize(input, output));

  assertTargetAreEqual(output, input);
}

TEST(Unit_Bsgalone_Core_Domain_Adapters_Messages_Serialization_TargetSerialization, ClearsTarget)
{
  Target input{.sourceDbId = Uuid{1234},
               .sourceKind = EntityKind::PLAYER,
               .targetDbId = Uuid{14},
               .targetKind = EntityKind::SHIP};

  Target output{.sourceDbId = Uuid{17640}, .sourceKind = EntityKind::OUTPOST};

  EXPECT_TRUE(serializeAndDeserialize(input, output));

  assertTargetAreEqual(output, input);
}

} // namespace bsgalone::core
