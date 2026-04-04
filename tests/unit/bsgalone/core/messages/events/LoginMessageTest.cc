
#include "LoginMessage.hh"
#include "SerializationHelper.hh"
#include <gtest/gtest.h>

using namespace test;

namespace bsgalone::core {
namespace {
void assertMessagesAreEqual(const LoginMessage &actual, const LoginMessage &expected)
{
  EXPECT_EQ(actual.type(), expected.type());
  EXPECT_EQ(actual.successfullyLoggedIn(), expected.successfullyLoggedIn());

  if (actual.successfullyLoggedIn())
  {
    EXPECT_EQ(actual.getPlayerDbId(), expected.getPlayerDbId());
    EXPECT_EQ(actual.getRole(), expected.getRole());
  }
}
} // namespace

TEST(Unit_Bsgalone_Core_Messages_Events_LoginMessage, IndicatesFailedLoginByDefault)
{
  LoginMessage message;

  EXPECT_FALSE(message.successfullyLoggedIn());
}

TEST(Unit_Bsgalone_Core_Messages_Events_LoginMessage, DoesNotDefinePlayerDataByDefault)
{
  LoginMessage message;

  EXPECT_THROW([&message]() { message.getPlayerDbId(); }(), std::bad_optional_access);
  EXPECT_THROW([&message]() { message.getRole(); }(), std::bad_optional_access);
}

TEST(Unit_Bsgalone_Core_Messages_Events_LoginMessage,
     IndicatesSuccessfulLoginWhenPlayerDataIsDefined)
{
  LoginMessage message;

  EXPECT_FALSE(message.successfullyLoggedIn());

  message.setPlayerDbId(Uuid{18});
  EXPECT_FALSE(message.successfullyLoggedIn());

  message.setRole(GameRole::PILOT);
  EXPECT_TRUE(message.successfullyLoggedIn());
}

TEST(Unit_Bsgalone_Core_Messages_Events_LoginMessage, ReturnsPlayerDataWhenSet)
{
  LoginMessage message;
  message.setPlayerDbId(Uuid{18});
  message.setRole(GameRole::PILOT);

  EXPECT_EQ(Uuid{18}, message.getPlayerDbId());
  EXPECT_EQ(GameRole::PILOT, message.getRole());
}

TEST(Unit_Bsgalone_Core_Messages_Events_LoginMessage, WithoutPlayerData)
{
  LoginMessage expected;

  const auto actual = serializeAndDeserializeMessage(expected);

  assertMessagesAreEqual(actual->as<LoginMessage>(), expected);
}

TEST(Unit_Bsgalone_Core_Messages_Events_LoginMessage, WithPlayerData)
{
  LoginMessage expected;
  expected.setPlayerDbId(Uuid{18});
  expected.setRole(GameRole::PILOT);

  const auto actual = serializeAndDeserializeMessage(expected);

  assertMessagesAreEqual(actual->as<LoginMessage>(), expected);
}

TEST(Unit_Bsgalone_Core_Messages_Events_LoginMessage, CloneWithoutPlayerData)
{
  LoginMessage expected;

  const auto cloned = expected.clone();

  ASSERT_EQ(cloned->type(), MessageType::LOGIN);
  assertMessagesAreEqual(cloned->as<LoginMessage>(), expected);
}

TEST(Unit_Bsgalone_Core_Messages_Credentials_LoginMessage, CloneWithPlayerData)
{
  LoginMessage expected;
  expected.setPlayerDbId(Uuid{18});
  expected.setRole(GameRole::PILOT);

  const auto cloned = expected.clone();

  ASSERT_EQ(cloned->type(), MessageType::LOGIN);
  assertMessagesAreEqual(cloned->as<LoginMessage>(), expected);
}

} // namespace bsgalone::core
