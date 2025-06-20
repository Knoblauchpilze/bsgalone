
#include "ShipData.hh"
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

auto assertDataAreEqual(const ShipData &actual, const ShipData &expected)
{
  EXPECT_EQ(actual.dbId, expected.dbId);
  EXPECT_EQ(actual.position, expected.position);
  EXPECT_EQ(actual.radius, expected.radius);
  EXPECT_EQ(actual.acceleration, expected.acceleration);
  EXPECT_EQ(actual.speed, expected.speed);
  EXPECT_EQ(actual.hullPoints, expected.hullPoints);
  EXPECT_EQ(actual.maxHullPoints, expected.maxHullPoints);
  EXPECT_EQ(actual.hullPointsRegen, expected.hullPointsRegen);
  EXPECT_EQ(actual.powerPoints, expected.powerPoints);
  EXPECT_EQ(actual.maxPowerPoints, expected.maxPowerPoints);
  EXPECT_EQ(actual.powerRegen, expected.powerRegen);
  EXPECT_EQ(actual.faction, expected.faction);
  EXPECT_EQ(actual.status, expected.status);
  EXPECT_EQ(actual.shipClass, expected.shipClass);
  EXPECT_EQ(actual.name, expected.name);
  EXPECT_EQ(actual.targetDbId, expected.targetDbId);
}
} // namespace

TEST(Unit_Bsgo_Serialization_ShipData, EqualWhenDbIdIsEqual)
{
  ShipData data1{.dbId           = Uuid{1234},
                 .position       = Eigen::Vector3f{1.0f, 2.0f, 3.0f},
                 .radius         = 5.0f,
                 .maxPowerPoints = 100.0f};

  ShipData data2{.dbId = Uuid{1234}, .powerRegen = 17.5f};

  EXPECT_TRUE(data1 == data2);
}

TEST(Unit_Bsgo_Serialization_ShipData, DifferentWhenDbIdIsDifferent)
{
  ShipData data1{.dbId     = Uuid{1234},
                 .position = Eigen::Vector3f{1.0f, 2.0f, 3.0f},
                 .radius   = 5.0f,
                 .status   = Status::SPECTATOR};

  ShipData data2 = data1;
  data2.dbId     = Uuid{5678};

  EXPECT_FALSE(data1 == data2);
}

TEST(Unit_Bsgo_Serialization_ShipData, Basic)
{
  ShipData input{.dbId     = Uuid{1234},
                 .position = Eigen::Vector3f{1.0f, 2.0f, 3.0f},
                 .radius   = 5.0f,
                 .name     = "my-ship"};

  ShipData output{.dbId = Uuid{14}, .powerRegen = 1.2345f, .shipClass = ShipClass::LINE};

  EXPECT_TRUE(serializeAndDeserializeMessage(input, output));

  assertDataAreEqual(output, input);
}

TEST(Unit_Bsgo_Serialization_ShipData, WithTarget)
{
  ShipData input{.dbId           = Uuid{1234},
                 .position       = Eigen::Vector3f{1.0f, 2.0f, 3.0f},
                 .radius         = 5.0f,
                 .maxPowerPoints = 100.0f,
                 .targetDbId     = Uuid{8901}};

  ShipData output{.dbId = Uuid{14}, .faction = Faction::CYLON, .status = Status::JUMP};

  EXPECT_TRUE(serializeAndDeserializeMessage(input, output));

  assertDataAreEqual(output, input);
}

} // namespace bsgo
