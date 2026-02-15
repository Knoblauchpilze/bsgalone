
#include "SystemDataMessage.hh"
#include "Comparison.hh"
#include <gtest/gtest.h>

using namespace ::testing;

namespace bsgo {
namespace {
void assertMessagesAreEqual(const SystemDataMessage &actual, const SystemDataMessage &expected)
{
  EXPECT_EQ(actual.type(), expected.type());
  EXPECT_EQ(actual.tryGetClientId(), expected.tryGetClientId());

  const auto actualTickData   = actual.getTickData();
  const auto expectedTickData = expected.getTickData();

  assertSystemTickDataAreEqual(actualTickData, expectedTickData);
}
} // namespace

TEST(Unit_Bsgo_Serialization_SystemDataMessage, Basic)
{
  SystemTickData data{
    .dbId        = Uuid{12},
    .currentTick = chrono::Tick::fromInt(14),
    .step        = chrono::TimeStep(2, chrono::Duration::fromSeconds(1.54f)),
  };
  const SystemDataMessage expected(data);

  data = SystemTickData{.dbId        = Uuid{15},
                        .currentTick = chrono::Tick(29.014f),
                        .step = chrono::TimeStep(78.98f, chrono::Duration::fromMilliseconds(1.54f))};
  SystemDataMessage actual(data);
  actual.setClientId(Uuid{2});

  serializeAndDeserializeMessage(expected, actual);

  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_SystemDataMessage, WithClientId)
{
  SystemTickData data{
    .dbId        = Uuid{85},
    .currentTick = chrono::Tick(12.04508f),
    .step        = chrono::TimeStep(9, chrono::Duration::fromMilliseconds(0.05074f)),
  };
  SystemDataMessage expected(data);
  expected.setClientId(Uuid{78});

  data = SystemTickData{.dbId        = Uuid{71},
                        .currentTick = chrono::Tick::fromInt(78),
                        .step = chrono::TimeStep(0.324f, chrono::Duration::fromSeconds(18741.035f))};
  SystemDataMessage actual(data);

  serializeAndDeserializeMessage(expected, actual);

  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_SystemDataMessage, Clone)
{
  SystemTickData data{
    .dbId        = Uuid{85},
    .currentTick = chrono::Tick(12.04508f),
    .step        = chrono::TimeStep(9, chrono::Duration::fromMilliseconds(0.05074f)),
  };

  const SystemDataMessage expected(data);

  const auto cloned = expected.clone();
  ASSERT_EQ(cloned->type(), bsgalone::core::MessageType::SYSTEM_DATA);

  assertMessagesAreEqual(cloned->as<SystemDataMessage>(), expected);
}

} // namespace bsgo
