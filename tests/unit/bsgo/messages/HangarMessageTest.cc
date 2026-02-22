
#include "HangarMessage.hh"
#include "Comparison.hh"
#include <gtest/gtest.h>

using namespace ::testing;

namespace bsgo {
namespace {
void assertMessagesAreEqual(const HangarMessage &actual, const HangarMessage &expected)
{
  EXPECT_EQ(actual.type(), expected.type());
  EXPECT_EQ(actual.tryGetClientId(), expected.tryGetClientId());
  assertPlayerShipDataAreEqual(actual.getShip(), expected.getShip());
}
} // namespace

TEST(Unit_Bsgo_Serialization_HangarMessage, WithShipDbId)
{
  const HangarMessage expected(Uuid{14});
  HangarMessage actual(Uuid{6});

  serializeAndDeserializeMessage(expected, actual);

  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_HangarMessage, WithShip)
{
  const PlayerShipData data{.dbId             = Uuid{8},
                            .radius           = 1.478f,
                            .hullPoints       = 98.54f,
                            .jumpTimeInThreat = chrono::TickDuration(45001.0f),
                            .weapons          = {{
                                                   .dbId       = Uuid{2},
                                                   .weaponDbId = Uuid{65},
                                                   .minDamage  = 45.87f,
                                        },
                                                 {.weaponDbId = Uuid{98},
                                                  .name       = "weapon 1",
                                                  .reloadTime = chrono::TickDuration::fromInt(45)}},
                            .computers
                            = {{.computerDbId   = Uuid{12},
                                .offensive      = true,
                                .powerCost      = 56.47f,
                                .range          = 14.78f,
                                .allowedTargets = std::unordered_set<
                                  bsgalone::core::EntityKind>{bsgalone::core::EntityKind::ASTEROID,
                                                              bsgalone::core::EntityKind::OUTPOST}},
                               {
                                 .dbId           = Uuid{27},
                                 .name           = "beefy computer",
                                 .reloadTime     = chrono::TickDuration(457.174f),
                                 .damageModifier = 45.1f,
                               }}};
  const HangarMessage expected(data);

  HangarMessage actual(Uuid{6});

  serializeAndDeserializeMessage(expected, actual);

  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_HangarMessage, ShipIdIsEqualToShipData)
{
  const PlayerShipData data{.dbId     = Uuid{42},
                            .docked   = true,
                            .jumpTime = chrono::TickDuration(4587.001f)};
  const HangarMessage message(data);

  EXPECT_EQ(message.getShipDbId(), data.dbId);
}

TEST(Unit_Bsgo_Serialization_HangarMessage, OverridesShipProperties)
{
  HangarMessage expected(Uuid{14});
  expected.setClientId(Uuid{26});

  const PlayerShipData data{.dbId             = Uuid{8},
                            .radius           = 1.478f,
                            .hullPoints       = 98.54f,
                            .jumpTimeInThreat = chrono::TickDuration(45001.7f),
                            .weapons          = {{
                                                   .dbId       = Uuid{2},
                                                   .weaponDbId = Uuid{65},
                                                   .minDamage  = 45.87f,
                                        },
                                                 {.weaponDbId = Uuid{98},
                                                  .name       = "weapon 1",
                                                  .reloadTime = chrono::TickDuration(45.712f)}},
                            .computers
                            = {{.computerDbId   = Uuid{12},
                                .offensive      = true,
                                .powerCost      = 56.47f,
                                .range          = 14.78f,
                                .allowedTargets = std::unordered_set<
                                  bsgalone::core::EntityKind>{bsgalone::core::EntityKind::ASTEROID,
                                                              bsgalone::core::EntityKind::OUTPOST}},
                               {
                                 .dbId           = Uuid{27},
                                 .name           = "beefy computer",
                                 .reloadTime     = chrono::TickDuration::fromInt(457),
                                 .damageModifier = 45.1f,
                               }}};
  HangarMessage actual(data);

  serializeAndDeserializeMessage(expected, actual);

  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_HangarMessage, WithClientId)
{
  const PlayerShipData data{};
  HangarMessage expected(data);
  expected.setClientId(Uuid{119});

  HangarMessage actual(Uuid{6});

  serializeAndDeserializeMessage(expected, actual);

  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_HangarMessage, CloneWithShipDbId)
{
  const HangarMessage expected(Uuid{14});

  const auto cloned = expected.clone();

  ASSERT_EQ(cloned->type(), bsgalone::core::MessageType::HANGAR);
  assertMessagesAreEqual(cloned->as<HangarMessage>(), expected);
}

TEST(Unit_Bsgo_Serialization_HangarMessage, CloneWithShip)
{
  const PlayerShipData data{.dbId       = Uuid{47},
                            .position   = Eigen::Vector3f(-4.74f, 32.154f, 981.032f),
                            .speed      = 26.578f,
                            .faction    = Faction::COLONIAL,
                            .name       = "vega missile",
                            .active     = true,
                            .playerDbId = Uuid{478},
                            .weapons    = {PlayerWeaponData{.dbId         = Uuid{67},
                                                            .slotPosition = Eigen::Vector3f(78.41f,
                                                                                         21.59f,
                                                                                         -65.418f),
                                                            .minDamage    = 1.54f,
                                                            .maxDamage    = 15.4f,
                                                            .range        = 6.58f}}};
  const HangarMessage expected(data);

  const auto cloned = expected.clone();

  ASSERT_EQ(cloned->type(), bsgalone::core::MessageType::HANGAR);
  assertMessagesAreEqual(cloned->as<HangarMessage>(), expected);
}

} // namespace bsgo
