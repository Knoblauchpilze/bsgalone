
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
                                         .jumpTimeInThreat = Tick(1265.89f)}};
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
                                   .jumpTime        = Tick(7456.10f),
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
     .jumpTimeInThreat = Tick(3224, 0.08f),
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
     .jumpTime       = Tick(5412, 0.9007f)},
    {
      .dbId             = Uuid{68},
      .hullPointsRegen  = 98.76f,
      .jumpTime         = Tick(741.01f),
      .jumpTimeInThreat = Tick(369, 0.502f),
      .slots            = {{Slot::COMPUTER, 5}},
    },
  };
  ShipListMessage expected(Faction::CYLON, shipsData);

  shipsData = {
    {.dbId          = Uuid{37},
     .shipClass     = ShipClass::STRIKE,
     .maxHullPoints = 14.47f,
     .radius        = 1.02f,
     .jumpTime      = Tick(5412.57f)},
    {.dbId             = Uuid{45},
     .shipClass        = ShipClass::LINE,
     .maxPowerPoints   = 21.74f,
     .acceleration     = 14.32f,
     .jumpTimeInThreat = Tick(3789, 0.14f)},
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
