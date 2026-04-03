
#include "SignupMessage.hh"
#include "SerializationHelper.hh"
#include <gtest/gtest.h>

using namespace test;

namespace bsgalone::core {
namespace {
void assertMessagesAreEqual(const SignupMessage &actual, const SignupMessage &expected)
{
  EXPECT_EQ(actual.type(), expected.type());
  EXPECT_EQ(actual.successfullySignedUp(), expected.successfullySignedUp());
}
} // namespace

TEST(Unit_Bsgalone_Core_Messages_Events_SignupMessage, CorrectlyReportsSignupStatus)
{
  SignupMessage failure(false);
  EXPECT_FALSE(failure.successfullySignedUp());

  SignupMessage success(true);
  EXPECT_TRUE(success.successfullySignedUp());
}

TEST(Unit_Bsgalone_Core_Messages_Events_LoginMessage, SerializationDeserialization)
{
  SignupMessage failure(true);
  auto actual = serializeAndDeserializeMessage(failure);
  assertMessagesAreEqual(actual->as<SignupMessage>(), failure);

  SignupMessage success(true);
  actual = serializeAndDeserializeMessage(success);
  assertMessagesAreEqual(actual->as<SignupMessage>(), success);
}

TEST(Unit_Bsgalone_Core_Messages_Credentials_LoginMessage, Clone)
{
  SignupMessage expected(true);

  const auto cloned = expected.clone();

  ASSERT_EQ(cloned->type(), MessageType::SIGNUP);
  assertMessagesAreEqual(cloned->as<SignupMessage>(), expected);
}

} // namespace bsgalone::core
