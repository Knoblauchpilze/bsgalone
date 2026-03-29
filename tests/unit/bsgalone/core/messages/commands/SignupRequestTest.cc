
#include "SignupRequest.hh"
#include "SerializationHelper.hh"
#include <gtest/gtest.h>

using namespace test;

namespace bsgalone::core {
namespace {
void assertMessagesAreEqual(const SignupRequest &actual, const SignupRequest &expected)
{
  EXPECT_EQ(actual.type(), expected.type());
  EXPECT_EQ(actual.getUsername(), expected.getUsername());
  EXPECT_EQ(actual.getPassword(), expected.getPassword());
  EXPECT_EQ(actual.getFaction(), expected.getFaction());
  EXPECT_EQ(actual.tryGetClientId(), expected.tryGetClientId());
}
} // namespace

TEST(Unit_Bsgalone_Core_Messages_Commands_SignupRequest, DoesNotDefineClientIdByDefault)
{
  const SignupRequest request("player", "secret", Faction::COLONIAL);

  EXPECT_FALSE(request.tryGetClientId().has_value());
}

TEST(Unit_Bsgalone_Core_Messages_Commands_SignupRequest, WithoutClientId)
{
  const SignupRequest expected("player", "secret", Faction::CYLON);

  const auto actual = serializeAndDeserializeMessage(expected);

  assertMessagesAreEqual(actual->as<SignupRequest>(), expected);
}

TEST(Unit_Bsgalone_Core_Messages_Commands_SignupRequest, WithClientId)
{
  SignupRequest expected("player", "secret", Faction::COLONIAL);
  expected.setClientId(net::ClientId{2});

  const auto actual = serializeAndDeserializeMessage(expected);

  assertMessagesAreEqual(actual->as<SignupRequest>(), expected);
}

TEST(Unit_Bsgalone_Core_Messages_Commands_SignupRequest, CloneWithoutClientId)
{
  const SignupRequest expected("player", "secret", Faction::CYLON);

  const auto cloned = expected.clone();

  ASSERT_EQ(cloned->type(), MessageType::SIGNUP_REQUEST);
  assertMessagesAreEqual(cloned->as<SignupRequest>(), expected);
}

TEST(Unit_Bsgalone_Core_Messages_Commands_SignupRequest, CloneWithClientId)
{
  SignupRequest expected("player", "secret", Faction::COLONIAL);
  expected.setClientId(net::ClientId{2});

  const auto cloned = expected.clone();

  ASSERT_EQ(cloned->type(), MessageType::SIGNUP_REQUEST);
  assertMessagesAreEqual(cloned->as<SignupRequest>(), expected);
}

} // namespace bsgalone::core
