
#include "LoadingStartedMessage.hh"
#include "Common.hh"
#include <gtest/gtest.h>

using namespace ::testing;

namespace bsgo {
namespace {
auto assertMessagesAreEqual(const LoadingStartedMessage &actual,
                            const LoadingStartedMessage &expected)
{
  EXPECT_EQ(actual.type(), expected.type());
  EXPECT_EQ(actual.tryGetClientId(), expected.tryGetClientId());

  EXPECT_EQ(actual.getSystemDbId(), expected.getSystemDbId());
  EXPECT_EQ(actual.getPlayerDbId(), expected.getPlayerDbId());
}
} // namespace

TEST(Unit_Bsgo_Serialization_LoadingStartedMessage, Basic)
{
  const LoadingStartedMessage expected(Uuid{1}, Uuid{2});

  LoadingStartedMessage actual(Uuid{89}, Uuid{36});
  actual.setClientId(Uuid{14});

  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_LoadingStartedMessage, WithClientId)
{
  LoadingStartedMessage expected(Uuid{18}, Uuid{789});
  expected.setClientId(Uuid{147});

  LoadingStartedMessage actual;

  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_LoadingStartedMessage, Clone)
{
  const LoadingStartedMessage expected(Uuid{587}, Uuid{734});
  const auto cloned = expected.clone();

  ASSERT_EQ(cloned->type(), MessageType::LOADING_STARTED);
  assertMessagesAreEqual(cloned->as<LoadingStartedMessage>(), expected);
}

} // namespace bsgo
