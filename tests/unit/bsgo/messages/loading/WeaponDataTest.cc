
#include "WeaponData.hh"
#include "Common.hh"
#include "Comparison.hh"
#include <gtest/gtest.h>

using namespace ::testing;

namespace bsgo {

TEST(Unit_Bsgo_Serialization_WeaponData, EqualWhenDbIdIsEqual)
{
  WeaponData data1{.dbId      = Uuid{1234},
                   .minDamage = 36.7f,
                   .range     = 123.456f,
                   .price     = {{Uuid{21}, 265}}};

  WeaponData data2{.dbId       = Uuid{1234},
                   .maxDamage  = 17.5f,
                   .reloadTime = chrono::TickDuration::fromInt(158)};

  EXPECT_TRUE(data1 == data2);
}

TEST(Unit_Bsgo_Serialization_WeaponData, DifferentWhenDbIdIsDifferent)
{
  WeaponData data1{.dbId = Uuid{1234}, .minDamage = 5.0f, .powerCost = 100.0f, .range = 0.1278f};

  WeaponData data2 = data1;
  data2.dbId       = Uuid{5678};

  EXPECT_FALSE(data1 == data2);
}

TEST(Unit_Bsgo_Serialization_WeaponData, Basic)
{
  WeaponData input{.dbId      = Uuid{1234},
                   .name      = "a weapon",
                   .minDamage = 5.4321f,
                   .range     = 98765.1234f,
                   .price     = {{Uuid{17}, 98}, {Uuid{3274}, 41}}};

  WeaponData output{.dbId       = Uuid{14},
                    .maxDamage  = 12.987f,
                    .reloadTime = chrono::TickDuration(1234.147f),
                    .price      = {{Uuid{26}, 401}, {Uuid{9874}, 130}, {Uuid{13}, 1245}}};

  EXPECT_TRUE(test::serializeAndDeserialize(input, output));

  assertWeaponDataAreEqual(output, input);
}

} // namespace bsgo
