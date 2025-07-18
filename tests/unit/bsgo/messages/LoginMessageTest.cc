
#include "LoginMessage.hh"
#include "Common.hh"
#include <gtest/gtest.h>

using namespace ::testing;

namespace bsgo {
namespace {
void assertMessagesAreEqual(const LoginMessage &actual, const LoginMessage &expected)
{
  EXPECT_EQ(actual.type(), expected.type());
  EXPECT_EQ(actual.getUserName(), expected.getUserName());
  EXPECT_EQ(actual.getUserPassword(), expected.getUserPassword());
  EXPECT_EQ(actual.getPlayerDbId(), expected.getPlayerDbId());
  EXPECT_EQ(actual.tryGetClientId(), expected.tryGetClientId());
  EXPECT_EQ(actual.validated(), expected.validated());
}
} // namespace

TEST(Unit_Bsgo_Serialization_LoginMessage, NameAndPassword)
{
  const LoginMessage expected("some-name", "some-password");
  LoginMessage actual("other-name", "secure-password", Uuid{4});
  actual.validate();
  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_LoginMessage, NameAndPassword_Validated)
{
  LoginMessage expected("some-name", "some-password");
  expected.validate();
  LoginMessage actual("other-name", "secure-password", Uuid{4});
  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_LoginMessage, NamePasswordPlayerDbId)
{
  const LoginMessage expected("some-name", "some-password", Uuid{14});
  LoginMessage actual("other-name", "secure-password");
  actual.validate();
  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_LoginMessage, NamePasswordPlayerDbId_Validated)
{
  LoginMessage expected("some-name", "some-password", Uuid{14});
  expected.validate();
  LoginMessage actual("other-name", "secure-password");
  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_LoginMessage, WithClientId)
{
  LoginMessage expected("some-name", "some-password", Uuid{14});
  expected.setClientId(Uuid{119});
  LoginMessage actual("other-name", "secure-password");
  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_LoginMessage, Clone)
{
  const LoginMessage expected("some-name", "some-password", Uuid{14});
  const auto cloned = expected.clone();
  ASSERT_EQ(cloned->type(), MessageType::LOGIN);
  assertMessagesAreEqual(cloned->as<LoginMessage>(), expected);
}

} // namespace bsgo
