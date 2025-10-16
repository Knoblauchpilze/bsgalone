
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
  EXPECT_EQ(actual.tryGetPlayerDbId(), expected.tryGetPlayerDbId());
}
} // namespace

TEST(Unit_Bsgo_Serialization_LoadingFinishedMessage, NoSystemNoPlayer)
{
  const LoadingFinishedMessage expected(LoadingTransition::JUMP);

  LoadingFinishedMessage actual(LoadingTransition::UNDOCK);
  actual.setSystemDbId(Uuid{1234});
  actual.setPlayerDbId(Uuid{7845});
  actual.setClientId(Uuid{12});

  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_LoadingFinishedMessage, WithSystem)
{
  LoadingFinishedMessage expected(LoadingTransition::PURCHASE);
  expected.setSystemDbId(Uuid{7845});

  LoadingFinishedMessage actual(LoadingTransition::LOGIN);
  actual.setPlayerDbId(Uuid{3456});

  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_LoadingFinishedMessage, WithPlayer)
{
  LoadingFinishedMessage expected(LoadingTransition::PURCHASE);
  expected.setPlayerDbId(Uuid{1234});

  LoadingFinishedMessage actual(LoadingTransition::LOGIN);
  actual.setSystemDbId(Uuid{8792});
  actual.setPlayerDbId(Uuid{3025});

  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_LoadingFinishedMessage, WithClientId)
{
  LoadingFinishedMessage expected(LoadingTransition::UNDOCK);
  expected.setPlayerDbId(Uuid{92});
  expected.setSystemDbId(Uuid{156});
  expected.setClientId(Uuid{17});

  LoadingFinishedMessage actual(LoadingTransition::LOGIN);

  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_LoadingFinishedMessage, CloneWithoutSystemWithoutPlayer)
{
  const LoadingFinishedMessage expected(LoadingTransition::LOGIN);
  const auto cloned = expected.clone();

  ASSERT_EQ(cloned->type(), MessageType::LOADING_FINISHED);
  assertMessagesAreEqual(cloned->as<LoadingFinishedMessage>(), expected);
}

TEST(Unit_Bsgo_Serialization_LoadingFinishedMessage, CloneWithSystemAndPlayer)
{
  LoadingFinishedMessage expected(LoadingTransition::JUMP);
  expected.setSystemDbId(Uuid{98});
  expected.setPlayerDbId(Uuid{57});

  const auto cloned = expected.clone();

  ASSERT_EQ(cloned->type(), MessageType::LOADING_FINISHED);
  assertMessagesAreEqual(cloned->as<LoadingFinishedMessage>(), expected);
}

} // namespace bsgo
