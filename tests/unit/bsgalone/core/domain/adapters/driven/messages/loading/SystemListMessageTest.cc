
#include "SystemListMessage.hh"
#include "Comparison.hh"
#include <gtest/gtest.h>

using namespace ::testing;

namespace bsgalone::core {
namespace {
void assertMessagesAreEqual(const SystemListMessage &actual, const SystemListMessage &expected)
{
  EXPECT_EQ(actual.type(), expected.type());

  const auto &actualSystemsData   = actual.getSystemsData();
  const auto &expectedSystemsData = expected.getSystemsData();

  EXPECT_EQ(actualSystemsData.size(), expectedSystemsData.size());
  for (std::size_t id = 0; id < actualSystemsData.size(); ++id)
  {
    const auto &actualSystemData   = actualSystemsData[id];
    const auto &expectedSystemData = expectedSystemsData[id];

    assertSystemDataAreEqual(actualSystemData, expectedSystemData);
  }

  EXPECT_EQ(actual.getPlayerDbId(), expected.getPlayerDbId());
  EXPECT_EQ(actual.getSystemDbId(), expected.getSystemDbId());
}
} // namespace

TEST(Unit_Bsgalone_Core_Domain_Adapters_Messages_Loading_SystemListMessage, Basic)
{
  const SystemListMessage expected(Uuid{18}, Uuid{19}, std::vector<SystemData>{});

  const std::vector<SystemData> systemsData{{.dbId = 23, .name = "system-1"},
                                            {.dbId     = 76,
                                             .name     = "some name",
                                             .position = Eigen::Vector3f(1.0f, 2.0f, 3.0f)}};

  const auto actual = serializeAndDeserializePlayerMessage(expected);

  assertMessagesAreEqual(actual->as<SystemListMessage>(), expected);
}

TEST(Unit_Bsgalone_Core_Domain_Adapters_Messages_Loading_SystemListMessage, Clone)
{
  const std::vector<SystemData> systemsData{
    {.dbId = 1908, .name = "a name", .position = Eigen::Vector3f::Ones(3)},
    {.dbId = 3207, .name = "pro gamer", .position = Eigen::Vector3f(-7.41f, -56.897f, -15879.21f)}};
  const SystemListMessage expected(Uuid{18}, Uuid{19}, systemsData);

  const auto cloned = expected.clone();

  ASSERT_EQ(cloned->type(), MessageType::SYSTEM_LIST);
  assertMessagesAreEqual(cloned->as<SystemListMessage>(), expected);
}

} // namespace bsgalone::core
