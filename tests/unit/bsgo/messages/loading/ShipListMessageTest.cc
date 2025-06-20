
#include "ShipListMessage.hh"
#include "Common.hh"
#include <gtest/gtest.h>

using namespace ::testing;

namespace bsgo {
namespace {
auto assertMessagesAreEqual(const ShipListMessage &actual, const ShipListMessage &expected)
{
  EXPECT_EQ(actual.type(), expected.type());
  EXPECT_EQ(actual.tryGetClientId(), expected.tryGetClientId());
  EXPECT_EQ(actual.getSystemDbId(), expected.getSystemDbId());

  const auto &actualShipsData   = actual.getShipsData();
  const auto &expectedShipsData = expected.getShipsData();

  EXPECT_EQ(actualShipsData, expectedShipsData);
}
} // namespace

TEST(Unit_Bsgo_Serialization_ShipListMessage, Basic)
{
  const ShipListMessage expected(Uuid{8712}, {});

  const std::vector<ShipData> outpostsData{{.dbId        = Uuid{23},
                                            .position    = Eigen::Vector3f(1.0f, 2.8f, 3.9f),
                                            .powerPoints = 11.3f},
                                           {.dbId       = Uuid{76},
                                            .radius     = 26.9f,
                                            .hullPoints = 100.0f,
                                            .targetDbId = Uuid{4567}}};
  ShipListMessage actual(Uuid{1515}, outpostsData);
  actual.setClientId(Uuid{2});
  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_ShipListMessage, WithClientId)
{
  std::vector<ShipData> outpostsData{{.dbId       = Uuid{65},
                                      .position   = Eigen::Vector3f(1.0f, 2.8f, 3.9f),
                                      .radius     = 26.9f,
                                      .hullPoints = 12.34f}};

  ShipListMessage expected(Uuid{123}, outpostsData);
  expected.setClientId(Uuid{78});

  outpostsData = {{.dbId = Uuid{17}, .powerPoints = 100.0f, .targetDbId = Uuid{923}},
                  {.dbId = Uuid{17}, .radius = 26.1}};
  ShipListMessage actual(Uuid{745}, {});
  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_ShipListMessage, Clone)
{
  const std::vector<ShipData> outpostsData{
    {.dbId = Uuid{23}, .position = Eigen::Vector3f(1.0f, 2.8f, 3.9f), .targetDbId = Uuid{4567}},
  };

  const ShipListMessage expected(Uuid{4572}, outpostsData);
  const auto cloned = expected.clone();
  ASSERT_EQ(cloned->type(), MessageType::SHIP_LIST);
  assertMessagesAreEqual(cloned->as<ShipListMessage>(), expected);
}
} // namespace bsgo
