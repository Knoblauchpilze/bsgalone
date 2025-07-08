
#include "AsteroidListMessage.hh"
#include "Common.hh"
#include <gtest/gtest.h>

using namespace ::testing;

namespace bsgo {
namespace {
void assertMessagesAreEqual(const AsteroidListMessage &actual, const AsteroidListMessage &expected)
{
  EXPECT_EQ(actual.type(), expected.type());
  EXPECT_EQ(actual.tryGetClientId(), expected.tryGetClientId());
  EXPECT_EQ(actual.getSystemDbId(), expected.getSystemDbId());

  const auto &actualAsteroidsData   = actual.getAsteroidsData();
  const auto &expectedAsteroidsData = expected.getAsteroidsData();

  EXPECT_EQ(actualAsteroidsData.size(), expectedAsteroidsData.size());
  for (std::size_t id = 0; id < actualAsteroidsData.size(); ++id)
  {
    const auto &actualAsteroidData   = actualAsteroidsData[id];
    const auto &expectedAsteroidData = expectedAsteroidsData[id];

    assertAsteroidDataAreEqual(actualAsteroidData, expectedAsteroidData);
  }
}
} // namespace

TEST(Unit_Bsgo_Serialization_AsteroidListMessage, Basic)
{
  const AsteroidListMessage expected(Uuid{8712}, {});

  const std::vector<AsteroidData> asteroidsData{{.dbId     = Uuid{23},
                                                 .position = Eigen::Vector3f(1.0f, 2.8f, 3.9f)},
                                                {.dbId     = Uuid{76},
                                                 .radius   = 26.9f,
                                                 .health   = 100.0f,
                                                 .resource = Uuid{4567},
                                                 .amount   = 11.3f}};
  AsteroidListMessage actual(Uuid{1515}, asteroidsData);
  actual.setClientId(Uuid{2});
  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_AsteroidListMessage, WithClientId)
{
  std::vector<AsteroidData> asteroidsData{{.dbId     = Uuid{65},
                                           .position = Eigen::Vector3f(1.0f, 2.8f, 3.9f),
                                           .radius   = 26.9f,
                                           .health   = 100.0f}};

  AsteroidListMessage expected(Uuid{123}, asteroidsData);
  expected.setClientId(Uuid{78});

  asteroidsData = {{.dbId = Uuid{17}, .health = 100.0f, .resource = Uuid{923}, .amount = 14.2f},
                   {.dbId = Uuid{17}, .radius = 26.1}};
  AsteroidListMessage actual(Uuid{745}, {});
  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_AsteroidListMessage, Clone)
{
  const std::vector<AsteroidData> asteroidsData{
    {.dbId = Uuid{23}, .position = Eigen::Vector3f(1.0f, 2.8f, 3.9f)},
  };

  const AsteroidListMessage expected(Uuid{4572}, asteroidsData);
  const auto cloned = expected.clone();
  ASSERT_EQ(cloned->type(), MessageType::ASTEROID_LIST);
  assertMessagesAreEqual(cloned->as<AsteroidListMessage>(), expected);
}
} // namespace bsgo
