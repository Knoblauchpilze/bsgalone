
#include "LoadingFinishedMessage.hh"
#include "Common.hh"
#include <gtest/gtest.h>

using namespace ::testing;

namespace bsgo {
namespace {
auto assertMessagesAreEqual(const LoadingFinishedMessage &actual,
                            const LoadingFinishedMessage &expected)
{
  EXPECT_EQ(actual.type(), expected.type());
  EXPECT_EQ(actual.tryGetClientId(), expected.tryGetClientId());
}
} // namespace

TEST(Unit_Bsgo_Serialization_LoadingFinishedMessage, Basic)
{
  const LoadingFinishedMessage expected;

  LoadingFinishedMessage actual;
  actual.setClientId(Uuid{2});

  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_LoadingFinishedMessage, WithClientId)
{
  LoadingFinishedMessage expected;
  expected.setClientId(Uuid{78});

  LoadingFinishedMessage actual;

  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_LoadingFinishedMessage, Clone)
{
  const LoadingFinishedMessage expected;
  const auto cloned = expected.clone();

  ASSERT_EQ(cloned->type(), MessageType::LOADING_FINISHED);
  assertMessagesAreEqual(cloned->as<LoadingFinishedMessage>(), expected);
}

} // namespace bsgo
