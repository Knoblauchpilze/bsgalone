
#include "ResourceListMessage.hh"
#include "Comparison.hh"
#include <gtest/gtest.h>

using namespace ::testing;

namespace bsgo {
namespace {
void assertMessagesAreEqual(const ResourceListMessage &actual, const ResourceListMessage &expected)
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

    assertResourceDataAreEqual(actualResourceData, expectedResourceData);
  }
}
} // namespace

TEST(Unit_Bsgo_Serialization_ResourceListMessage, Basic)
{
  const ResourceListMessage expected(std::vector<ResourceData>{});

  const std::vector<ResourceData> resourcesData{{.dbId = 23, .name = "resource-1"},
                                                {.dbId = 76, .name = "some name"}};
  ResourceListMessage actual(resourcesData);
  actual.setClientId(Uuid{2});
  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_ResourceListMessage, WithClientId)
{
  std::vector<ResourceData> playersData{{.dbId = 14, .name = "foo bar"}};

  ResourceListMessage expected(playersData);
  expected.setClientId(Uuid{78});

  ResourceListMessage actual(std::vector<ResourceData>{});
  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_ResourceListMessage, Clone)
{
  const std::vector<ResourceData> resourcesData{{.dbId = 1908, .name = "a name"},
                                                {.dbId = 3207, .name = "valuable resource"}};

  const ResourceListMessage expected(resourcesData);
  const auto cloned = expected.clone();
  ASSERT_EQ(cloned->type(), MessageType::RESOURCE_LIST);
  assertMessagesAreEqual(cloned->as<ResourceListMessage>(), expected);
}

} // namespace bsgo
