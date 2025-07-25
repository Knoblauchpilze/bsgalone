
#include "Common.hh"
#include "DataSerialization.hh"
#include <gtest/gtest.h>

using namespace ::testing;

namespace bsgo {
namespace {
inline bool serializeAndDeserializeData(const PlayerWeaponData &value, PlayerWeaponData &output)
{
  std::ostringstream out{};
  serializePlayerWeaponData(out, value);
  std::istringstream in(out.str());
  return deserializePlayerWeaponData(in, output);
}
} // namespace

TEST(Unit_Bsgo_Serialization_PlayerWeaponData, EqualWhenDbIdIsEqual)
{
  PlayerWeaponData data1{.dbId       = Uuid{1234},
                         .weaponDbId = Uuid{41},
                         .minDamage  = 36.7f,
                         .range      = 123.456f};

  PlayerWeaponData data2{.dbId       = Uuid{1234},
                         .level      = 5,
                         .maxDamage  = 17.5f,
                         .reloadTime = core::toMilliseconds(158)};

  EXPECT_TRUE(data1 == data2);
}

TEST(Unit_Bsgo_Serialization_PlayerWeaponData, DifferentWhenDbIdIsDifferent)
{
  PlayerWeaponData data1{.dbId       = Uuid{1234},
                         .weaponDbId = Uuid{67},
                         .minDamage  = 5.0f,
                         .powerCost  = 100.0f};

  PlayerWeaponData data2 = data1;
  data2.dbId             = Uuid{5678};

  EXPECT_FALSE(data1 == data2);
}

TEST(Unit_Bsgo_Serialization_PlayerWeaponData, Basic)
{
  PlayerWeaponData input{.dbId         = Uuid{1234},
                         .slotPosition = Eigen::Vector3f{1.2f, 4.3f, -5.7f},
                         .level        = 4,
                         .minDamage    = 5.4321f,
                         .range        = 98765.1234f};

  PlayerWeaponData output{.dbId       = Uuid{14},
                          .maxDamage  = 12.987f,
                          .reloadTime = core::toMilliseconds(1234)};

  EXPECT_TRUE(serializeAndDeserializeData(input, output));

  assertPlayerWeaponDataAreEqual(output, input);
}

} // namespace bsgo
