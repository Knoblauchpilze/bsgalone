
#include "SignupMessage.hh"
#include "Comparison.hh"
#include <gtest/gtest.h>

using namespace ::testing;

namespace bsgo {
namespace {
void assertMessagesAreEqual(const SignupMessage &actual, const SignupMessage &expected)
{
  EXPECT_EQ(actual.type(), expected.type());
  EXPECT_EQ(actual.getUserName(), expected.getUserName());
  EXPECT_EQ(actual.getUserPassword(), expected.getUserPassword());
  EXPECT_EQ(actual.getFaction(), expected.getFaction());
  EXPECT_EQ(actual.getPlayerDbId(), expected.getPlayerDbId());
  EXPECT_EQ(actual.tryGetClientId(), expected.tryGetClientId());
}
} // namespace

TEST(Unit_Bsgo_Serialization_SignupMessage, NoPlayerDbId)
{
  const SignupMessage expected("some-name", "some-password", Faction::COLONIAL);
  SignupMessage actual("some-other-name", "securepassword", Faction::CYLON, Uuid{26});

  serializeAndDeserializeMessage(expected, actual);

  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_SignupMessage, WithPlayerDbId)
{
  const SignupMessage expected("some-name", "some-password", Faction::COLONIAL, Uuid{44});
  SignupMessage actual("some-other-name", "securepassword", Faction::CYLON);

  serializeAndDeserializeMessage(expected, actual);

  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_SignupMessage, WithClientId)
{
  SignupMessage expected("some-name", "some-password", Faction::COLONIAL, Uuid{44});
  expected.setClientId(Uuid{119});
  SignupMessage actual("some-other-name", "securepassword", Faction::CYLON);

  serializeAndDeserializeMessage(expected, actual);

  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_SignupMessage, Clone)
{
  const SignupMessage expected("some-name", "some-password", Faction::COLONIAL, Uuid{44});

  const auto cloned = expected.clone();

  ASSERT_EQ(cloned->type(), MessageType::SIGNUP);
  assertMessagesAreEqual(cloned->as<SignupMessage>(), expected);
}

} // namespace bsgo
