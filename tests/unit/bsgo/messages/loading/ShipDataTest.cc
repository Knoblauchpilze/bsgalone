
#include "Common.hh"
#include "DataSerialization.hh"
#include <gtest/gtest.h>

using namespace ::testing;

namespace bsgo {
namespace {
inline bool serializeAndDeserializeMessage(const ShipData &value, ShipData &output)
{
  std::ostringstream out{};
  serializeShipData(out, value);
  std::istringstream in(out.str());
  return deserializeShipData(in, output);
}
} // namespace

TEST(Unit_Bsgo_Serialization_ShipData, EqualWhenDbIdIsEqual)
{
  ShipData data1{.dbId           = Uuid{1234},
                 .faction        = Faction::COLONIAL,
                 .maxPowerPoints = 100.0f,
                 .radius         = 5.0f};

  ShipData data2{.dbId = Uuid{1234}, .powerRegen = 17.5f};

  EXPECT_TRUE(data1 == data2);
}

TEST(Unit_Bsgo_Serialization_ShipData, DifferentWhenDbIdIsDifferent)
{
  ShipData data1{.dbId      = Uuid{1234},
                 .shipClass = ShipClass::STRIKE,
                 .radius    = 5.0f,
                 .jumpTime  = chrono::TickDuration(529.0f),
                 .slots     = {{Slot::COMPUTER, 2}}};

  ShipData data2 = data1;
  data2.dbId     = Uuid{5678};

  EXPECT_FALSE(data1 == data2);
}

TEST(Unit_Bsgo_Serialization_ShipData, Basic)
{
  ShipData input{
    .dbId            = Uuid{1234},
    .name            = "my-ship",
    .maxHullPoints   = 3247.0f,
    .hullPointsRegen = 5.5f,
    .radius          = 5.0f,
  };

  ShipData output{.dbId             = Uuid{14},
                  .shipClass        = ShipClass::LINE,
                  .maxPowerPoints   = 78.954f,
                  .powerRegen       = 1.2345f,
                  .jumpTimeInThreat = chrono::TickDuration(1234.238f)};

  EXPECT_TRUE(serializeAndDeserializeMessage(input, output));

  assertShipDataAreEqual(output, input);
}

TEST(Unit_Bsgo_Serialization_ShipData, WithSlots)
{
  ShipData input{.dbId           = Uuid{1234},
                 .maxPowerPoints = 100.0f,
                 .acceleration   = 2.5f,
                 .speed          = 1.78f,
                 .radius         = 5.0f,
                 .slots          = {{Slot::COMPUTER, 2}, {Slot::WEAPON, 4}}};

  ShipData output{.dbId      = Uuid{14},
                  .faction   = Faction::CYLON,
                  .shipClass = ShipClass::LINE,
                  .name      = "the whale",
                  .slots     = {{Slot::COMPUTER, 5}}};

  EXPECT_TRUE(serializeAndDeserializeMessage(input, output));

  assertShipDataAreEqual(output, input);
}

TEST(Unit_Bsgo_Serialization_ShipData, WithPrice)
{
  ShipData input{.dbId           = Uuid{1234},
                 .maxPowerPoints = 100.0f,
                 .acceleration   = 2.5f,
                 .speed          = 1.78f,
                 .radius         = 5.0f,
                 .price          = {{Uuid{1}, 100}, {Uuid{2}, 200}}};

  ShipData output{.dbId      = Uuid{14},
                  .faction   = Faction::CYLON,
                  .shipClass = ShipClass::LINE,
                  .name      = "the whale",
                  .price     = {{Uuid{8}, 14758}, {Uuid{9}, 36098}, {Uuid{5}, 1247}}};

  EXPECT_TRUE(serializeAndDeserializeMessage(input, output));

  assertShipDataAreEqual(output, input);
}

} // namespace bsgo
