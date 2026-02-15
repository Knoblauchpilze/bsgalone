
#include "PlayerWeaponListMessage.hh"
#include "Comparison.hh"
#include <gtest/gtest.h>

using namespace ::testing;

namespace bsgo {
namespace {
void assertMessagesAreEqual(const PlayerWeaponListMessage &actual,
                            const PlayerWeaponListMessage &expected)
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

    assertPlayerWeaponDataAreEqual(actualWeaponData, expectedWeaponData);
  }
}
} // namespace

TEST(Unit_Bsgo_Serialization_PlayerWeaponListMessage, Basic)
{
  const PlayerWeaponListMessage expected(std::vector<PlayerWeaponData>{});

  const std::vector<PlayerWeaponData> weaponsData{
    {.dbId = 23, .range = 1.456f, .reloadTime = chrono::TickDuration::fromInt(1546)},
    {.dbId = 76, .slotPosition = Eigen::Vector3f(-7.8f, 45.12f, -0.458f), .minDamage = 14}};
  PlayerWeaponListMessage actual(weaponsData);
  actual.setClientId(Uuid{2});

  serializeAndDeserializeMessage(expected, actual);

  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_PlayerWeaponListMessage, WithClientId)
{
  std::vector<PlayerWeaponData> weaponsData{
    {.dbId = 14, .weaponDbId = 26, .level = 1, .powerCost = 14.56f}};

  PlayerWeaponListMessage expected(weaponsData);
  expected.setClientId(Uuid{78});

  weaponsData = {{.weaponDbId = 2, .powerCost = 1.457f},
                 {.maxDamage  = 1.78f,
                  .range      = 5.64f,
                  .reloadTime = chrono::TickDuration(7891.5413f)}};
  PlayerWeaponListMessage actual(weaponsData);

  serializeAndDeserializeMessage(expected, actual);

  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_PlayerWeaponListMessage, Clone)
{
  const std::vector<PlayerWeaponData> weaponsData{
    {.weaponDbId = 1908, .level = 12, .minDamage = 7.451f},
    {.level = 3, .powerCost = -3.9878f, .reloadTime = chrono::TickDuration(15001.03874f)}};

  const PlayerWeaponListMessage expected(weaponsData);

  const auto cloned = expected.clone();

  ASSERT_EQ(cloned->type(), bsgalone::core::MessageType::PLAYER_WEAPON_LIST);
  assertMessagesAreEqual(cloned->as<PlayerWeaponListMessage>(), expected);
}

} // namespace bsgo
