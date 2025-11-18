
#include "OutpostListMessage.hh"
#include "Comparison.hh"
#include <gtest/gtest.h>

using namespace ::testing;

namespace bsgo {
namespace {
void assertMessagesAreEqual(const OutpostListMessage &actual, const OutpostListMessage &expected)
{
  EXPECT_EQ(actual.type(), expected.type());
  EXPECT_EQ(actual.tryGetClientId(), expected.tryGetClientId());
  EXPECT_EQ(actual.getSystemDbId(), expected.getSystemDbId());

  const auto &actualOutpostsData   = actual.getOutpostsData();
  const auto &expectedOutpostsData = expected.getOutpostsData();

  EXPECT_EQ(actualOutpostsData.size(), expectedOutpostsData.size());
  for (std::size_t id = 0; id < actualOutpostsData.size(); ++id)
  {
    const auto &actualOutpostData   = actualOutpostsData[id];
    const auto &expectedOutpostData = expectedOutpostsData[id];

    assertOutpostDataAreEqual(actualOutpostData, expectedOutpostData);
  }
}
} // namespace

TEST(Unit_Bsgo_Serialization_OutpostListMessage, Basic)
{
  const OutpostListMessage expected(Uuid{8712}, {});

  const std::vector<OutpostData> outpostsData{{.dbId        = Uuid{23},
                                               .position    = Eigen::Vector3f(1.0f, 2.8f, 3.9f),
                                               .powerPoints = 11.3f},
                                              {.dbId       = Uuid{76},
                                               .radius     = 26.9f,
                                               .hullPoints = 100.0f}};
  OutpostListMessage actual(Uuid{1515}, outpostsData);
  actual.setClientId(Uuid{2});
  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_OutpostListMessage, WithClientId)
{
  std::vector<OutpostData> outpostsData{{.dbId       = Uuid{65},
                                         .position   = Eigen::Vector3f(1.0f, 2.8f, 3.9f),
                                         .radius     = 26.9f,
                                         .hullPoints = 12.34f}};

  OutpostListMessage expected(Uuid{123}, outpostsData);
  expected.setClientId(Uuid{78});

  outpostsData = {{.dbId = Uuid{17}, .powerPoints = 100.0f}, {.dbId = Uuid{17}, .radius = 26.1}};
  OutpostListMessage actual(Uuid{745}, {});
  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_OutpostListMessage, Clone)
{
  const std::vector<OutpostData> outpostsData{
    {.dbId = Uuid{23}, .position = Eigen::Vector3f(1.0f, 2.8f, 3.9f)},
  };

  const OutpostListMessage expected(Uuid{4572}, outpostsData);
  const auto cloned = expected.clone();
  ASSERT_EQ(cloned->type(), MessageType::OUTPOST_LIST);
  assertMessagesAreEqual(cloned->as<OutpostListMessage>(), expected);
}
} // namespace bsgo
