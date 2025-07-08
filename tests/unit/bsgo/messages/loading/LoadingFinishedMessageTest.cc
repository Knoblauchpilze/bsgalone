
#include "LoadingFinishedMessage.hh"
#include "Common.hh"
#include <gtest/gtest.h>

using namespace ::testing;

namespace bsgo {
namespace {
void assertMessagesAreEqual(const LoadingFinishedMessage &actual,
                            const LoadingFinishedMessage &expected)
{
  EXPECT_EQ(actual.type(), expected.type());
  EXPECT_EQ(actual.tryGetClientId(), expected.tryGetClientId());
  EXPECT_EQ(actual.getTransition(), expected.getTransition());
  EXPECT_EQ(actual.getSystemDbId(), expected.getSystemDbId());
  EXPECT_EQ(actual.tryGetPlayerDbId(), expected.tryGetPlayerDbId());
}
} // namespace

TEST(Unit_Bsgo_Serialization_LoadingFinishedMessage, Basic)
{
  const LoadingFinishedMessage expected(LoadingTransition::JUMP, Uuid{1234}, Uuid{7845});

  LoadingFinishedMessage actual(LoadingTransition::UNDOCK, Uuid{4});
  actual.setClientId(Uuid{12});

  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_LoadingFinishedMessage, WithoutPlayerDbId)
{
  const LoadingFinishedMessage expected(LoadingTransition::LOGIN, Uuid{32});

  LoadingFinishedMessage actual(LoadingTransition::JUMP, Uuid{489}, Uuid{6578});

  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_LoadingFinishedMessage, WithClientId)
{
  LoadingFinishedMessage expected(LoadingTransition::UNDOCK, Uuid{92}, Uuid{156});
  expected.setClientId(Uuid{17});

  LoadingFinishedMessage actual(LoadingTransition::LOGIN, Uuid{10}, Uuid{7});

  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_LoadingFinishedMessage, Clone)
{
  const LoadingFinishedMessage expected(LoadingTransition::LOGIN, Uuid{98}, Uuid{57});
  const auto cloned = expected.clone();

  ASSERT_EQ(cloned->type(), MessageType::LOADING_FINISHED);
  assertMessagesAreEqual(cloned->as<LoadingFinishedMessage>(), expected);
}

TEST(Unit_Bsgo_Serialization_LoadingFinishedMessage, CloneWithoutPlayerDbId)
{
  const LoadingFinishedMessage expected(LoadingTransition::JUMP, Uuid{37});
  const auto cloned = expected.clone();

  ASSERT_EQ(cloned->type(), MessageType::LOADING_FINISHED);
  assertMessagesAreEqual(cloned->as<LoadingFinishedMessage>(), expected);
}

} // namespace bsgo
