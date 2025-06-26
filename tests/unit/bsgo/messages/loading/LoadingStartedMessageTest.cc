
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
  EXPECT_EQ(actual.getTransition(), expected.getTransition());
  EXPECT_EQ(actual.getSystemDbId(), expected.getSystemDbId());
  EXPECT_EQ(actual.tryGetPlayerDbId(), expected.tryGetPlayerDbId());
}
} // namespace

TEST(Unit_Bsgo_Serialization_LoadingStartedMessage, Basic)
{
  const LoadingStartedMessage expected(LoadingTransition::JUMP, Uuid{1234}, Uuid{7845});

  LoadingStartedMessage actual(LoadingTransition::UNDOCK, Uuid{4});
  actual.setClientId(Uuid{12});

  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_LoadingStartedMessage, WithoutPlayerDbId)
{
  const LoadingStartedMessage expected(LoadingTransition::LOGIN, Uuid{32});

  LoadingStartedMessage actual(LoadingTransition::JUMP, Uuid{489}, Uuid{6578});

  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_LoadingStartedMessage, WithClientId)
{
  LoadingStartedMessage expected(LoadingTransition::UNDOCK, Uuid{92}, Uuid{156});
  expected.setClientId(Uuid{17});

  LoadingStartedMessage actual(LoadingTransition::LOGIN, Uuid{10}, Uuid{7});

  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_LoadingStartedMessage, Clone)
{
  const LoadingStartedMessage expected(LoadingTransition::JUMP, Uuid{98}, Uuid{57});
  const auto cloned = expected.clone();

  ASSERT_EQ(cloned->type(), MessageType::LOADING_STARTED);
  assertMessagesAreEqual(cloned->as<LoadingStartedMessage>(), expected);
}

TEST(Unit_Bsgo_Serialization_LoadingStartedMessage, CloneWithoutPlayerDbId)
{
  const LoadingStartedMessage expected(LoadingTransition::LOGIN, Uuid{37});
  const auto cloned = expected.clone();

  ASSERT_EQ(cloned->type(), MessageType::LOADING_STARTED);
  assertMessagesAreEqual(cloned->as<LoadingStartedMessage>(), expected);
}

} // namespace bsgo
