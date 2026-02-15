
#include "WeaponListMessage.hh"
#include "Comparison.hh"
#include <gtest/gtest.h>

using namespace ::testing;

namespace bsgo {
namespace {
void assertMessagesAreEqual(const WeaponListMessage &actual, const WeaponListMessage &expected)
{
  EXPECT_EQ(actual.type(), expected.type());
  EXPECT_EQ(actual.tryGetClientId(), expected.tryGetClientId());

  const auto &actualWeaponsData   = actual.getWeaponsData();
  const auto &expectedWeaponsData = expected.getWeaponsData();

  EXPECT_EQ(actualWeaponsData.size(), expectedWeaponsData.size());
  for (std::size_t id = 0; id < actualWeaponsData.size(); ++id)
  {
    const auto &actualWeaponData   = actualWeaponsData[id];
    const auto &expectedWeaponData = expectedWeaponsData[id];

    assertWeaponDataAreEqual(actualWeaponData, expectedWeaponData);
  }
}
} // namespace

TEST(Unit_Bsgo_Serialization_WeaponListMessage, Basic)
{
  const WeaponListMessage expected(std::vector<WeaponData>{});

  const std::vector<WeaponData> weaponsData{{.dbId       = Uuid{23},
                                             .range      = 1.456f,
                                             .reloadTime = chrono::TickDuration::fromInt(1546)},
                                            {.dbId      = Uuid{76},
                                             .minDamage = 14,
                                             .powerCost = 3.547f}};
  WeaponListMessage actual(weaponsData);
  actual.setClientId(Uuid{2});

  serializeAndDeserializeMessage(expected, actual);

  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_WeaponListMessage, WithClientId)
{
  std::vector<WeaponData> weaponsData{{.dbId = Uuid{14}, .name = "big gun", .powerCost = 14.56f}};

  WeaponListMessage expected(weaponsData);
  expected.setClientId(Uuid{78});

  weaponsData = {{.dbId = Uuid{2}, .powerCost = 1.457f},
                 {.maxDamage = 1.78f, .range = 5.64f, .reloadTime = chrono::TickDuration(7891.24f)}};
  WeaponListMessage actual(weaponsData);

  serializeAndDeserializeMessage(expected, actual);

  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_WeaponListMessage, Clone)
{
  const std::vector<WeaponData> weaponsData{{.dbId      = Uuid{1908},
                                             .name      = "the gun",
                                             .minDamage = 7.451f},
                                            {.maxDamage  = 0.2357f,
                                             .powerCost  = -3.9878f,
                                             .reloadTime = chrono::TickDuration(15001.00479805f)}};

  const WeaponListMessage expected(weaponsData);

  const auto cloned = expected.clone();

  ASSERT_EQ(cloned->type(), bsgalone::core::MessageType::WEAPON_LIST);
  assertMessagesAreEqual(cloned->as<WeaponListMessage>(), expected);
}

} // namespace bsgo
