
#include "Common.hh"
#include "DataSerialization.hh"
#include <gtest/gtest.h>

using namespace ::testing;

namespace bsgo {
namespace {
inline bool serializeAndDeserializeData(const WeaponData &value, WeaponData &output)
{
  std::ostringstream out{};
  serializeWeaponData(out, value);
  std::istringstream in(out.str());
  return deserializeWeaponData(in, output);
}
} // namespace

TEST(Unit_Bsgo_Serialization_WeaponData, EqualWhenDbIdIsEqual)
{
  WeaponData data1{.dbId      = Uuid{1234},
                   .minDamage = 36.7f,
                   .range     = 123.456f,
                   .price     = {{Uuid{21}, 26.5f}}};

  WeaponData data2{.dbId = Uuid{1234}, .maxDamage = 17.5f, .reloadTime = core::toMilliseconds(158)};

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
                   .price     = {{Uuid{17}, 98.032f}, {Uuid{3274}, 41.097f}}};

  WeaponData output{.dbId       = Uuid{14},
                    .maxDamage  = 12.987f,
                    .reloadTime = core::toMilliseconds(1234),
                    .price = {{Uuid{26}, 401.298f}, {Uuid{9874}, 130.27f}, {Uuid{13}, 12.45f}}};

  EXPECT_TRUE(serializeAndDeserializeData(input, output));

  assertWeaponDataAreEqual(output, input);
}

} // namespace bsgo
