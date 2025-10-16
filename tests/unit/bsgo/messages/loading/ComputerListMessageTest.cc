
#include "ComputerListMessage.hh"
#include "Comparison.hh"
#include <gtest/gtest.h>

using namespace ::testing;

namespace bsgo {
namespace {
void assertMessagesAreEqual(const ComputerListMessage &actual, const ComputerListMessage &expected)
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

    assertComputerDataAreEqual(actualComputerData, expectedComputerData);
  }
}
} // namespace

TEST(Unit_Bsgo_Serialization_ComputerListMessage, Basic)
{
  const ComputerListMessage expected(std::vector<ComputerData>{});

  const std::vector<ComputerData> computersData{{.dbId           = 23,
                                                 .offensive      = true,
                                                 .damageModifier = -47.89f},
                                                {.dbId           = 76,
                                                 .duration       = chrono::TickDuration(360.7324f),
                                                 .damageModifier = 75.4f}};
  ComputerListMessage actual(computersData);
  actual.setClientId(Uuid{2});
  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_ComputerListMessage, WithClientId)
{
  std::vector<ComputerData> computersData{
    {.dbId           = 14,
     .powerCost      = 14.56f,
     .reloadTime     = chrono::TickDuration::fromInt(26),
     .allowedTargets = std::unordered_set<EntityKind>{EntityKind::OUTPOST}}};

  ComputerListMessage expected(computersData);
  expected.setClientId(Uuid{78});

  ComputerListMessage actual(std::vector<ComputerData>{});
  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_ComputerListMessage, Clone)
{
  const std::vector<ComputerData>
    computersData{{.dbId           = 1908,
                   .offensive      = true,
                   .allowedTargets = std::unordered_set<EntityKind>{EntityKind::ASTEROID}},
                  {.name       = "beefy computer",
                   .powerCost  = -3.9878f,
                   .reloadTime = chrono::TickDuration::fromInt(15001)}};

  const ComputerListMessage expected(computersData);

  const auto cloned = expected.clone();

  ASSERT_EQ(cloned->type(), MessageType::COMPUTER_LIST);
  assertMessagesAreEqual(cloned->as<ComputerListMessage>(), expected);
}

} // namespace bsgo
