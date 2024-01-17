
#include "LoginMessage.hh"
#include "Common.hh"
#include <gtest/gtest.h>

using namespace ::testing;

namespace bsgo {
namespace {
auto assertMessagesAreEqual(const LoginMessage &actual, const LoginMessage &expected)
{
  EXPECT_EQ(actual.type(), expected.type());
  EXPECT_EQ(actual.getLoginType(), expected.getLoginType());
  EXPECT_EQ(actual.getUserName(), expected.getUserName());
  EXPECT_EQ(actual.getUserPassword(), expected.getUserPassword());
  EXPECT_EQ(actual.getFaction(), expected.getFaction());
  EXPECT_EQ(actual.getLoginState(), expected.getLoginState());
  EXPECT_EQ(actual.getPlayerId(), expected.getPlayerId());
}
} // namespace

TEST(Unit_Bsgo_Serialization_LoginMessage, NameAndPassword)
{
  const LoginMessage expected("some-name", "some-password");
  LoginMessage actual(LoginType::LOGIN, Uuid{4}, LoginState::VALIDATED);
  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_LoginMessage, NamePasswordAndFaction)
{
  const LoginMessage expected("another-name", "secure-password", Faction::COLONIAL);
  LoginMessage actual("somebody-else", "not-so-secure");
  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_LoginMessage, WithLoginType)
{
  const LoginMessage expected(LoginType::SIGNUP, Uuid{30}, LoginState::VALIDATED);
  LoginMessage actual("some-guy", "password");
  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);
}

} // namespace bsgo
