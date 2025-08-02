
#include "PlayerComputerListMessage.hh"
#include "Common.hh"
#include <gtest/gtest.h>

using namespace ::testing;

namespace bsgo {
namespace {
void assertMessagesAreEqual(const PlayerComputerListMessage &actual,
                            const PlayerComputerListMessage &expected)
{
  EXPECT_EQ(actual.type(), expected.type());
  EXPECT_EQ(actual.tryGetClientId(), expected.tryGetClientId());

  const auto &actualComputersData   = actual.getComputersData();
  const auto &expectedComputersData = expected.getComputersData();

  EXPECT_EQ(actualComputersData.size(), expectedComputersData.size());
  for (std::size_t id = 0; id < actualComputersData.size(); ++id)
  {
    const auto &actualComputerData   = actualComputersData[id];
    const auto &expectedComputerData = expectedComputersData[id];

    assertPlayerComputerDataAreEqual(actualComputerData, expectedComputerData);
  }
}
} // namespace

TEST(Unit_Bsgo_Serialization_PlayerComputerListMessage, Basic)
{
  const PlayerComputerListMessage expected(std::vector<PlayerComputerData>{});

  const std::vector<PlayerComputerData>
    computersData{{.dbId = 23, .offensive = true, .damageModifier = -47.89f},
                  {.dbId = 76, .level = 14, .duration = core::toMilliseconds(360)}};
  PlayerComputerListMessage actual(computersData);
  actual.setClientId(Uuid{2});
  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_PlayerComputerListMessage, WithClientId)
{
  std::vector<PlayerComputerData> computersData{
    {.dbId = 14, .computerDbId = 26, .powerCost = 14.56f}};

  PlayerComputerListMessage expected(computersData);
  expected.setClientId(Uuid{78});

  PlayerComputerListMessage actual(std::vector<PlayerComputerData>{});
  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_PlayerComputerListMessage, Clone)
{
  const std::vector<PlayerComputerData> computersData{
    {.computerDbId   = 1908,
     .level          = 12,
     .allowedTargets = std::unordered_set<EntityKind>{EntityKind::ASTEROID}},
    {.offensive = true, .powerCost = -3.9878f, .reloadTime = core::toMilliseconds(15001)}};

  const PlayerComputerListMessage expected(computersData);

  const auto cloned = expected.clone();

  ASSERT_EQ(cloned->type(), MessageType::PLAYER_COMPUTER_LIST);
  assertMessagesAreEqual(cloned->as<PlayerComputerListMessage>(), expected);
}

} // namespace bsgo
