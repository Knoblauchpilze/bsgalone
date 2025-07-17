
#include "SystemListMessage.hh"
#include "Common.hh"
#include <gtest/gtest.h>

using namespace ::testing;

namespace bsgo {
namespace {
void assertMessagesAreEqual(const SystemListMessage &actual, const SystemListMessage &expected)
{
  EXPECT_EQ(actual.type(), expected.type());
  EXPECT_EQ(actual.tryGetClientId(), expected.tryGetClientId());

  const auto &actualSystemsData   = actual.getSystemsData();
  const auto &expectedSystemsData = expected.getSystemsData();

  EXPECT_EQ(actualSystemsData.size(), expectedSystemsData.size());
  for (std::size_t id = 0; id < actualSystemsData.size(); ++id)
  {
    const auto &actualSystemData   = actualSystemsData[id];
    const auto &expectedSystemData = expectedSystemsData[id];

    assertSystemDataAreEqual(actualSystemData, expectedSystemData);
  }
}
} // namespace

TEST(Unit_Bsgo_Serialization_SystemListMessage, Basic)
{
  const SystemListMessage expected(std::vector<SystemData>{});

  const std::vector<SystemData> systemsData{{.dbId = 23, .name = "system-1"},
                                            {.dbId     = 76,
                                             .name     = "some name",
                                             .position = Eigen::Vector3f(1.0f, 2.0f, 3.0f)}};
  SystemListMessage actual(systemsData);
  actual.setClientId(Uuid{2});
  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_SystemListMessage, WithClientId)
{
  std::vector<SystemData> systemsData{{.dbId = 14, .name = "foo bar"}};

  SystemListMessage expected(systemsData);
  expected.setClientId(Uuid{78});

  systemsData = {{.dbId = 87, .position = Eigen::Vector3f(1.45f, 3.258f, -8.0f)},
                 {.dbId = 923, .name = "ground breaking"}};
  SystemListMessage actual(systemsData);
  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_SystemListMessage, Clone)
{
  const std::vector<SystemData> systemsData{
    {.dbId = 1908, .name = "a name", .position = Eigen::Vector3f::Ones(3)},
    {.dbId = 3207, .name = "pro gamer", .position = Eigen::Vector3f(-7.41f, -56.897f, -15879.21f)}};

  const SystemListMessage expected(systemsData);
  const auto cloned = expected.clone();
  ASSERT_EQ(cloned->type(), MessageType::SYSTEM_LIST);
  assertMessagesAreEqual(cloned->as<SystemListMessage>(), expected);
}

} // namespace bsgo
