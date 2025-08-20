
#include "ShipListMessage.hh"
#include "Common.hh"
#include <gtest/gtest.h>

using namespace ::testing;

namespace bsgo {
namespace {
void assertMessagesAreEqual(const ShipListMessage &actual, const ShipListMessage &expected)
{
  EXPECT_EQ(actual.type(), expected.type());
  EXPECT_EQ(actual.tryGetClientId(), expected.tryGetClientId());
  EXPECT_EQ(actual.getFaction(), expected.getFaction());

  const auto &actualShipsData   = actual.getShipsData();
  const auto &expectedShipsData = expected.getShipsData();

  EXPECT_EQ(actualShipsData.size(), expectedShipsData.size());
  for (std::size_t id = 0; id < actualShipsData.size(); ++id)
  {
    const auto &actualShipData   = actualShipsData[id];
    const auto &expectedShipData = expectedShipsData[id];

    assertShipDataAreEqual(actualShipData, expectedShipData);
  }
}
} // namespace

TEST(Unit_Bsgo_Serialization_ShipListMessage, Basic)
{
  const ShipListMessage expected(Faction::CYLON, std::vector<ShipData>{});

  const std::vector<ShipData> shipsData{{.dbId    = Uuid{23},
                                         .faction = Faction::CYLON,
                                         .name    = "ship 1"},
                                        {.dbId             = Uuid{76},
                                         .maxHullPoints    = 100.0f,
                                         .hullPointsRegen  = 26.9f,
                                         .jumpTimeInThreat = core::toMilliseconds(1265)}};
  ShipListMessage actual(Faction::COLONIAL, shipsData);
  actual.setClientId(Uuid{2});

  serializeAndDeserializeMessage(expected, actual);

  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_ShipListMessage, WithClientId)
{
  std::vector<ShipData> shipsData{{.dbId            = Uuid{65},
                                   .shipClass       = ShipClass::LINE,
                                   .hullPointsRegen = 12.34f,
                                   .maxPowerPoints  = 56.78f,
                                   .jumpTime        = core::toMilliseconds(7456),
                                   .slots           = {{Slot::COMPUTER, 2}, {Slot::WEAPON, 3}}}};

  ShipListMessage expected(Faction::COLONIAL, shipsData);
  expected.setClientId(Uuid{78});

  shipsData = {{
                 .dbId           = Uuid{17},
                 .maxPowerPoints = 100.0f,
                 .speed          = 78.45f,
                 .radius         = 1.54f,
               },
               {.dbId = Uuid{18}, .radius = 26.1, .slots = {{Slot::COMPUTER, 2}}}};
  ShipListMessage actual(Faction::COLONIAL, shipsData);

  serializeAndDeserializeMessage(expected, actual);

  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_ShipListMessage, Clone)
{
  const std::vector<ShipData> shipsData{
    {.dbId             = Uuid{23},
     .shipClass        = ShipClass::STRIKE,
     .name             = "another ship",
     .acceleration     = 1.257f,
     .jumpTimeInThreat = core::toMilliseconds(3224),
     .slots            = {{Slot::WEAPON, 14}}},
  };

  ShipListMessage expected(Faction::CYLON, shipsData);

  const auto cloned = expected.clone();

  ASSERT_EQ(cloned->type(), MessageType::SHIP_LIST);
  assertMessagesAreEqual(cloned->as<ShipListMessage>(), expected);
}

TEST(Unit_Bsgo_Serialization_ShipListMessage, MultipleComplexShips)
{
  std::vector<ShipData> shipsData{
    {.dbId           = Uuid{65},
     .shipClass      = ShipClass::LINE,
     .name           = "ship 1",
     .maxHullPoints  = 14.47f,
     .maxPowerPoints = 89.45f,
     .powerRegen     = 1.051f,
     .acceleration   = 17.45f,
     .speed          = 23.65f,
     .radius         = 1.02f,
     .jumpTime       = core::toMilliseconds(5412)},
    {
      .dbId             = Uuid{68},
      .hullPointsRegen  = 98.76f,
      .jumpTime         = core::toMilliseconds(741),
      .jumpTimeInThreat = core::toMilliseconds(369),
      .slots            = {{Slot::COMPUTER, 5}},
    },
  };
  ShipListMessage expected(Faction::CYLON, shipsData);

  shipsData = {
    {.dbId          = Uuid{37},
     .shipClass     = ShipClass::STRIKE,
     .maxHullPoints = 14.47f,
     .radius        = 1.02f,
     .jumpTime      = core::toMilliseconds(5412)},
    {.dbId             = Uuid{45},
     .shipClass        = ShipClass::LINE,
     .maxPowerPoints   = 21.74f,
     .acceleration     = 14.32f,
     .jumpTimeInThreat = core::toMilliseconds(3789)},
    {.dbId      = Uuid{41},
     .shipClass = ShipClass::STRIKE,
     .name      = "the mosquito",
     .speed     = 14.32f,
     .slots     = {{Slot::WEAPON, 5}, {Slot::COMPUTER, 5}}},
  };
  ShipListMessage actual(Faction::COLONIAL, shipsData);

  serializeAndDeserializeMessage(expected, actual);

  assertMessagesAreEqual(actual, expected);
}
} // namespace bsgo
