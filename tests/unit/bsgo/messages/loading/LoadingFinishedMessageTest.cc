
#include "LoadingFinishedMessage.hh"
#include "Comparison.hh"
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
  EXPECT_EQ(actual.tryGetSystemDbId(), expected.tryGetSystemDbId());
  EXPECT_EQ(actual.getPlayerDbId(), expected.getPlayerDbId());
}
} // namespace

TEST(Unit_Bsgo_Serialization_LoadingFinishedMessage, NoSystem)
{
  const LoadingFinishedMessage expected(LoadingTransition::JUMP, Uuid{7845});

  LoadingFinishedMessage actual(LoadingTransition::UNDOCK, Uuid{17});
  actual.setSystemDbId(Uuid{1234});
  actual.setClientId(Uuid{12});

  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_LoadingFinishedMessage, WithSystem)
{
  LoadingFinishedMessage expected(LoadingTransition::PURCHASE, Uuid{13});
  expected.setSystemDbId(Uuid{7845});

  LoadingFinishedMessage actual(LoadingTransition::LOGIN, Uuid{3456});

  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_LoadingFinishedMessage, OverridesSystem)
{
  LoadingFinishedMessage expected(LoadingTransition::PURCHASE, Uuid{1234});

  LoadingFinishedMessage actual(LoadingTransition::LOGIN, Uuid{3025});
  actual.setSystemDbId(Uuid{8792});

  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_LoadingFinishedMessage, WithClientId)
{
  LoadingFinishedMessage expected(LoadingTransition::UNDOCK, Uuid{92});
  expected.setSystemDbId(Uuid{156});
  expected.setClientId(Uuid{17});

  LoadingFinishedMessage actual(LoadingTransition::LOGIN, Uuid{870});

  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_LoadingFinishedMessage, CloneWithoutSystem)
{
  const LoadingFinishedMessage expected(LoadingTransition::LOGIN, Uuid{1243});
  const auto cloned = expected.clone();

  ASSERT_EQ(cloned->type(), bsgalone::core::MessageType::LOADING_FINISHED);
  assertMessagesAreEqual(cloned->as<LoadingFinishedMessage>(), expected);
}

TEST(Unit_Bsgo_Serialization_LoadingFinishedMessage, CloneWithSystem)
{
  LoadingFinishedMessage expected(LoadingTransition::JUMP, Uuid{57});
  expected.setSystemDbId(Uuid{98});

  const auto cloned = expected.clone();

  ASSERT_EQ(cloned->type(), bsgalone::core::MessageType::LOADING_FINISHED);
  assertMessagesAreEqual(cloned->as<LoadingFinishedMessage>(), expected);
}

} // namespace bsgo
