
#include "PlayerResourceListMessage.hh"
#include "Common.hh"
#include <gtest/gtest.h>

using namespace ::testing;

namespace bsgo {
namespace {
void assertMessagesAreEqual(const PlayerResourceListMessage &actual,
                            const PlayerResourceListMessage &expected)
{
  EXPECT_EQ(actual.type(), expected.type());
  EXPECT_EQ(actual.tryGetClientId(), expected.tryGetClientId());

  const auto &actualResourcesData   = actual.getResourcesData();
  const auto &expectedResourcesData = expected.getResourcesData();

  EXPECT_EQ(actualResourcesData.size(), expectedResourcesData.size());
  for (std::size_t id = 0; id < actualResourcesData.size(); ++id)
  {
    const auto &actualResourceData   = actualResourcesData[id];
    const auto &expectedResourceData = expectedResourcesData[id];

    assertPlayerResourceDataAreEqual(actualResourceData, expectedResourceData);
  }
}
} // namespace

TEST(Unit_Bsgo_Serialization_PlayerResourceListMessage, Basic)
{
  const PlayerResourceListMessage expected(std::vector<PlayerResourceData>{});

  const std::vector<PlayerResourceData>
    resourcesData{{.dbId = 23, .name = "resource-1", .amount = -47.89f},
                  {.dbId = 76, .name = "some name", .amount = 1.45f}};
  PlayerResourceListMessage actual(resourcesData);
  actual.setClientId(Uuid{2});
  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_PlayerResourceListMessage, WithClientId)
{
  std::vector<PlayerResourceData> resourcesData{{.dbId = 14, .name = "foo bar", .amount = 14.56f}};

  PlayerResourceListMessage expected(resourcesData);
  expected.setClientId(Uuid{78});

  PlayerResourceListMessage actual(std::vector<PlayerResourceData>{});
  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_PlayerResourceListMessage, Clone)
{
  const std::vector<PlayerResourceData>
    resourcesData{{.dbId = 1908, .name = "a name", .amount = 0.14754f},
                  {.dbId = 3207, .name = "valuable resource", .amount = -3.9878f}};

  const PlayerResourceListMessage expected(resourcesData);
  const auto cloned = expected.clone();
  ASSERT_EQ(cloned->type(), MessageType::PLAYER_RESOURCE_LIST);
  assertMessagesAreEqual(cloned->as<PlayerResourceListMessage>(), expected);
}

} // namespace bsgo
