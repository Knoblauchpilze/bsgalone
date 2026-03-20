
#include "LoginMessage.hh"
#include "Comparison.hh"
#include <gtest/gtest.h>

using namespace ::testing;

namespace bsgalone::core {
namespace {
void assertMessagesAreEqual(const LoginMessage &actual, const LoginMessage &expected)
{
  EXPECT_EQ(actual.type(), expected.type());
  EXPECT_EQ(actual.getClientId(), expected.getClientId());
  EXPECT_EQ(actual.successfullyLoggedIn(), expected.successfullyLoggedIn());

  if (actual.successfullyLoggedIn())
  {
    EXPECT_EQ(actual.getPlayerDbId(), expected.getPlayerDbId());
    EXPECT_EQ(actual.getRole(), expected.getRole());
    EXPECT_EQ(actual.getSystemDbId(), expected.getSystemDbId());
  }
}
} // namespace

TEST(Unit_Bsgalone_Core_Messages_Credentials_LoginMessage, IndicatesFailedLoginByDefault)
{
  LoginMessage message(net::ClientId{12});

  EXPECT_FALSE(message.successfullyLoggedIn());
}

TEST(Unit_Bsgalone_Core_Messages_Credentials_LoginMessage, DoesNotDefinePlayerDataByDefault)
{
  LoginMessage message(net::ClientId{12});

  EXPECT_THROW([&message]() { message.getPlayerDbId(); }(), std::bad_optional_access);
  EXPECT_THROW([&message]() { message.getRole(); }(), std::bad_optional_access);
  EXPECT_THROW([&message]() { message.getSystemDbId(); }(), std::bad_optional_access);
}

TEST(Unit_Bsgalone_Core_Messages_Credentials_LoginMessage,
     IndicatesSuccessfulLoginWhenPlayerDataIsDefined)
{
  LoginMessage message(net::ClientId{12});

  EXPECT_FALSE(message.successfullyLoggedIn());

  message.setPlayerDbId(Uuid{18});
  EXPECT_FALSE(message.successfullyLoggedIn());

  message.setRole(GameRole::PILOT);
  EXPECT_FALSE(message.successfullyLoggedIn());

  message.setSystemDbId(Uuid{19});
  EXPECT_TRUE(message.successfullyLoggedIn());
}

TEST(Unit_Bsgalone_Core_Messages_Credentials_LoginMessage, ReturnsPlayerDataWhenSet)
{
  LoginMessage message(net::ClientId{12});
  message.setPlayerDbId(Uuid{18});
  message.setRole(GameRole::PILOT);
  message.setSystemDbId(Uuid{19});

  EXPECT_EQ(Uuid{18}, message.getPlayerDbId());
  EXPECT_EQ(GameRole::PILOT, message.getRole());
  EXPECT_EQ(Uuid{19}, message.getSystemDbId());
}

TEST(Unit_Bsgalone_Core_Messages_Credentials_LoginMessage, WithoutPlayerData)
{
  LoginMessage expected(net::ClientId{12});

  const auto actual = serializeAndDeserializePlayerMessage(expected);

  assertMessagesAreEqual(actual->as<LoginMessage>(), expected);
}

TEST(Unit_Bsgalone_Core_Messages_Credentials_LoginMessage, WithPlayerData)
{
  LoginMessage expected(net::ClientId{12});
  expected.setPlayerDbId(Uuid{18});
  expected.setRole(GameRole::PILOT);
  expected.setSystemDbId(Uuid{19});

  const auto actual = serializeAndDeserializePlayerMessage(expected);

  assertMessagesAreEqual(actual->as<LoginMessage>(), expected);
}

TEST(Unit_Bsgalone_Core_Messages_Credentials_LoginMessage, CloneWithoutPlayerData)
{
  LoginMessage expected(net::ClientId{12});

  const auto cloned = expected.clone();

  ASSERT_EQ(cloned->type(), MessageType::LOGIN);
  assertMessagesAreEqual(cloned->as<LoginMessage>(), expected);
}

TEST(Unit_Bsgalone_Core_Messages_Credentials_LoginMessage, CloneWithPlayerData)
{
  LoginMessage expected(net::ClientId{12});
  expected.setPlayerDbId(Uuid{18});
  expected.setRole(GameRole::PILOT);
  expected.setSystemDbId(Uuid{19});

  const auto cloned = expected.clone();

  ASSERT_EQ(cloned->type(), MessageType::LOGIN);
  assertMessagesAreEqual(cloned->as<LoginMessage>(), expected);
}

} // namespace bsgalone::core
