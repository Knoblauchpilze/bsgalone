
#include "WeaponData.hh"
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

auto assertDataAreEqual(const WeaponData &actual, const WeaponData &expected)
{
  EXPECT_EQ(actual.dbId, expected.dbId);
  EXPECT_EQ(actual.weaponDbId, expected.weaponDbId);
  EXPECT_EQ(actual.slotPosition, expected.slotPosition);
  EXPECT_EQ(actual.level, expected.level);
  EXPECT_EQ(actual.minDamage, expected.minDamage);
  EXPECT_EQ(actual.maxDamage, expected.maxDamage);
  EXPECT_EQ(actual.powerCost, expected.powerCost);
  EXPECT_EQ(actual.range, expected.range);
  EXPECT_EQ(actual.reloadTime, expected.reloadTime);
}
} // namespace

TEST(Unit_Bsgo_Serialization_WeaponData, EqualWhenDbIdIsEqual)
{
  WeaponData data1{.dbId       = Uuid{1234},
                   .weaponDbId = Uuid{41},
                   .minDamage  = 36.7f,
                   .range      = 123.456f};

  WeaponData data2{.dbId       = Uuid{1234},
                   .level      = 5,
                   .maxDamage  = 17.5f,
                   .reloadTime = core::toMilliseconds(158)};

  EXPECT_TRUE(data1 == data2);
}

TEST(Unit_Bsgo_Serialization_WeaponData, DifferentWhenDbIdIsDifferent)
{
  WeaponData data1{.dbId       = Uuid{1234},
                   .weaponDbId = Uuid{67},
                   .minDamage  = 5.0f,
                   .powerCost  = 100.0f};

  WeaponData data2 = data1;
  data2.dbId       = Uuid{5678};

  EXPECT_FALSE(data1 == data2);
}

TEST(Unit_Bsgo_Serialization_WeaponData, Basic)
{
  WeaponData input{.dbId         = Uuid{1234},
                   .slotPosition = Eigen::Vector3f{1.2f, 4.3f, -5.7f},
                   .level        = 4,
                   .minDamage    = 5.4321f,
                   .range        = 98765.1234f};

  WeaponData output{.dbId       = Uuid{14},
                    .maxDamage  = 12.987f,
                    .reloadTime = core::toMilliseconds(1234)};

  EXPECT_TRUE(serializeAndDeserializeData(input, output));

  assertDataAreEqual(output, input);
}

} // namespace bsgo
