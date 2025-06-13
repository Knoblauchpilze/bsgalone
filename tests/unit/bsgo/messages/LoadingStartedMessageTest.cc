
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
  const LoadingStartedMessage expected(Uuid{1234}, Uuid{7845});

  LoadingStartedMessage actual(Uuid{4}, Uuid{54});
  actual.setClientId(Uuid{12});

  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_LoadingStartedMessage, WithClientId)
{
  LoadingStartedMessage expected(Uuid{92}, Uuid{156});
  expected.setClientId(Uuid{17});

  LoadingStartedMessage actual(Uuid{10}, Uuid{7});

  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_LoadingStartedMessage, Clone)
{
  const LoadingStartedMessage expected(Uuid{98}, Uuid{57});
  const auto cloned = expected.clone();

  ASSERT_EQ(cloned->type(), MessageType::LOADING_STARTED);
  assertMessagesAreEqual(cloned->as<LoadingStartedMessage>(), expected);
}

} // namespace bsgo
