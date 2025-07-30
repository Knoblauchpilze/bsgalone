
#include "LoadingStartedMessage.hh"
#include "Common.hh"
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
  EXPECT_EQ(actual.tryGetPlayerDbId(), expected.tryGetPlayerDbId());
}
} // namespace

TEST(Unit_Bsgo_Serialization_LoadingStartedMessage, NoSystemNoPlayer)
{
  const LoadingStartedMessage expected(LoadingTransition::JUMP);

  LoadingStartedMessage actual(LoadingTransition::UNDOCK);
  actual.setSystemDbId(Uuid{1234});
  actual.setPlayerDbId(Uuid{7845});
  actual.setClientId(Uuid{12});

  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_LoadingStartedMessage, WithSystem)
{
  LoadingStartedMessage expected(LoadingTransition::PURCHASE);
  expected.setSystemDbId(Uuid{7845});

  LoadingStartedMessage actual(LoadingTransition::LOGIN);
  actual.setPlayerDbId(Uuid{3456});

  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_LoadingStartedMessage, WithPlayer)
{
  LoadingStartedMessage expected(LoadingTransition::PURCHASE);
  expected.setPlayerDbId(Uuid{1234});

  LoadingStartedMessage actual(LoadingTransition::LOGIN);
  actual.setSystemDbId(Uuid{8792});
  actual.setPlayerDbId(Uuid{3025});

  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_LoadingStartedMessage, WithClientId)
{
  LoadingStartedMessage expected(LoadingTransition::UNDOCK);
  expected.setPlayerDbId(Uuid{92});
  expected.setSystemDbId(Uuid{156});
  expected.setClientId(Uuid{17});

  LoadingStartedMessage actual(LoadingTransition::LOGIN);

  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_LoadingStartedMessage, CloneWithoutSystemWithoutPlayer)
{
  const LoadingStartedMessage expected(LoadingTransition::LOGIN);
  const auto cloned = expected.clone();

  ASSERT_EQ(cloned->type(), MessageType::LOADING_STARTED);
  assertMessagesAreEqual(cloned->as<LoadingStartedMessage>(), expected);
}

TEST(Unit_Bsgo_Serialization_LoadingStartedMessage, CloneWithSystemAndPlayer)
{
  LoadingStartedMessage expected(LoadingTransition::JUMP);
  expected.setSystemDbId(Uuid{98});
  expected.setPlayerDbId(Uuid{57});

  const auto cloned = expected.clone();

  ASSERT_EQ(cloned->type(), MessageType::LOADING_STARTED);
  assertMessagesAreEqual(cloned->as<LoadingStartedMessage>(), expected);
}

} // namespace bsgo
