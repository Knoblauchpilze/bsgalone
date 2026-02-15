
#include "LoadingStartedMessage.hh"
#include "Comparison.hh"
#include <gtest/gtest.h>

using namespace ::testing;

namespace bsgo {
namespace {
void assertMessagesAreEqual(const LoadingStartedMessage &actual,
                            const LoadingStartedMessage &expected)
{
  EXPECT_EQ(actual.type(), expected.type());
  EXPECT_EQ(actual.tryGetClientId(), expected.tryGetClientId());
  EXPECT_EQ(actual.getTransition(), expected.getTransition());
  EXPECT_EQ(actual.tryGetSystemDbId(), expected.tryGetSystemDbId());
  EXPECT_EQ(actual.getPlayerDbId(), expected.getPlayerDbId());
}
} // namespace

TEST(Unit_Bsgo_Serialization_LoadingStartedMessage, NoSystem)
{
  const LoadingStartedMessage expected(LoadingTransition::JUMP, Uuid{12});

  LoadingStartedMessage actual(LoadingTransition::UNDOCK, Uuid{7845});
  actual.setSystemDbId(Uuid{1234});
  actual.setClientId(Uuid{12});

  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_LoadingStartedMessage, WithSystem)
{
  LoadingStartedMessage expected(LoadingTransition::PURCHASE, Uuid{13});
  expected.setSystemDbId(Uuid{7845});

  LoadingStartedMessage actual(LoadingTransition::LOGIN, Uuid{3456});

  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_LoadingStartedMessage, OverridesSystem)
{
  LoadingStartedMessage expected(LoadingTransition::PURCHASE, Uuid{1234});

  LoadingStartedMessage actual(LoadingTransition::LOGIN, Uuid{3025});
  actual.setSystemDbId(Uuid{8792});

  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_LoadingStartedMessage, WithClientId)
{
  LoadingStartedMessage expected(LoadingTransition::UNDOCK, Uuid{92});
  expected.setSystemDbId(Uuid{156});
  expected.setClientId(Uuid{17});

  LoadingStartedMessage actual(LoadingTransition::LOGIN, Uuid{870});

  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_LoadingStartedMessage, CloneWithoutSystem)
{
  const LoadingStartedMessage expected(LoadingTransition::LOGIN, Uuid{1243});
  const auto cloned = expected.clone();

  ASSERT_EQ(cloned->type(), bsgalone::core::MessageType::LOADING_STARTED);
  assertMessagesAreEqual(cloned->as<LoadingStartedMessage>(), expected);
}

TEST(Unit_Bsgo_Serialization_LoadingStartedMessage, CloneWithSystem)
{
  LoadingStartedMessage expected(LoadingTransition::JUMP, Uuid{57});
  expected.setSystemDbId(Uuid{98});

  const auto cloned = expected.clone();

  ASSERT_EQ(cloned->type(), bsgalone::core::MessageType::LOADING_STARTED);
  assertMessagesAreEqual(cloned->as<LoadingStartedMessage>(), expected);
}

} // namespace bsgo
