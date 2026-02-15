
#include "LoginMessage.hh"
#include "Comparison.hh"
#include <gtest/gtest.h>

using namespace ::testing;

namespace bsgo {
namespace {
void assertMessagesAreEqual(const LoginMessage &actual, const LoginMessage &expected)
{
  EXPECT_EQ(actual.type(), expected.type());
  EXPECT_EQ(actual.getUserName(), expected.getUserName());
  EXPECT_EQ(actual.getPassword(), expected.getPassword());
  EXPECT_EQ(actual.getGameRole(), expected.getGameRole());
  EXPECT_EQ(actual.tryGetPlayerDbId(), expected.tryGetPlayerDbId());
  EXPECT_EQ(actual.tryGetSystemDbId(), expected.tryGetSystemDbId());
  EXPECT_EQ(actual.tryGetClientId(), expected.tryGetClientId());
}
} // namespace

TEST(Unit_Bsgo_Serialization_LoginMessage, ClearsPlayerCredentials)
{
  LoginMessage expected(GameRole::GUNNER);

  LoginMessage actual(GameRole::PILOT);
  actual.setUserName("other-name");
  actual.setPassword("secure-password");

  serializeAndDeserializeMessage(expected, actual);

  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_LoginMessage, ClearsPlayerDbId)
{
  LoginMessage expected(GameRole::GUNNER);
  expected.setUserName("some-name");
  expected.setPassword("some-password");

  LoginMessage actual(GameRole::PILOT);
  actual.setUserName("other-name");
  actual.setPassword("secure-password");
  actual.setPlayerDbId(Uuid{4});

  serializeAndDeserializeMessage(expected, actual);

  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_LoginMessage, OverridesPlayerDbId)
{
  LoginMessage expected(GameRole::PILOT);
  expected.setUserName("some-name");
  expected.setPassword("some-password");
  expected.setPlayerDbId(Uuid{4});

  LoginMessage actual(GameRole::GUNNER);
  actual.setUserName("other-name");
  actual.setPassword("secure-password");

  serializeAndDeserializeMessage(expected, actual);

  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_LoginMessage, OverridesSystemDbId)
{
  LoginMessage expected(GameRole::GUNNER);
  expected.setSystemDbId(Uuid{17});

  LoginMessage actual(GameRole::PILOT);
  actual.setSystemDbId(Uuid{31});

  serializeAndDeserializeMessage(expected, actual);

  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_LoginMessage, WithClientId)
{
  LoginMessage expected(GameRole::PILOT);
  expected.setUserName("some-name");
  expected.setPassword("some-password");
  expected.setClientId(Uuid{119});

  LoginMessage actual(GameRole::PILOT);
  actual.setUserName("other-name");
  actual.setPassword("secure-password");

  serializeAndDeserializeMessage(expected, actual);

  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_LoginMessage, Clone)
{
  LoginMessage expected(GameRole::GUNNER);
  expected.setUserName("some-name");
  expected.setPassword("some-password");
  expected.setClientId(Uuid{119});
  expected.setPlayerDbId(Uuid{14});

  const auto cloned = expected.clone();

  ASSERT_EQ(cloned->type(), bsgalone::core::MessageType::LOGIN);
  assertMessagesAreEqual(cloned->as<LoginMessage>(), expected);
}

} // namespace bsgo
