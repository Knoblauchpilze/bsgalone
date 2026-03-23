
#include "LoginRequest.hh"
#include "Comparison.hh"
#include "SerializationHelper.hh"
#include <gtest/gtest.h>

using namespace test;
using namespace ::testing;

namespace bsgalone::core {
namespace {
void assertMessagesAreEqual(const LoginRequest &actual, const LoginRequest &expected)
{
  EXPECT_EQ(actual.type(), expected.type());
  EXPECT_EQ(actual.getUsername(), expected.getUsername());
  EXPECT_EQ(actual.getPassword(), expected.getPassword());
  EXPECT_EQ(actual.getRole(), expected.getRole());
  EXPECT_EQ(actual.tryGetClientId(), expected.tryGetClientId());
}
} // namespace

TEST(Unit_Bsgalone_Core_Messages_Commands_LoginRequest, DoesNotDefineClientIdByDefault)
{
  const LoginRequest request("player", "secret", GameRole::GUNNER);

  EXPECT_FALSE(request.tryGetClientId().has_value());
}

TEST(Unit_Bsgalone_Core_Messages_Commands_LoginRequest, WithoutClientId)
{
  const LoginRequest expected("player", "secret", GameRole::GUNNER);

  const auto actual = serializeAndDeserializePlayerMessage(expected);

  assertMessagesAreEqual(actual->as<LoginRequest>(), expected);
}

TEST(Unit_Bsgalone_Core_Messages_Commands_LoginRequest, WithClientId)
{
  LoginRequest expected("player", "secret", GameRole::GUNNER);
  expected.setClientId(net::ClientId{2});

  const auto actual = serializeAndDeserializePlayerMessage(expected);

  assertMessagesAreEqual(actual->as<LoginRequest>(), expected);
}

TEST(Unit_Bsgalone_Core_Messages_Commands_LoginRequest, CloneWithoutClientId)
{
  const LoginRequest expected("player", "secret", GameRole::GUNNER);

  const auto cloned = expected.clone();

  ASSERT_EQ(cloned->type(), MessageType::LOGIN_REQUEST);
  assertMessagesAreEqual(cloned->as<LoginRequest>(), expected);
}

TEST(Unit_Bsgalone_Core_Messages_Commands_LoginRequest, CloneWithClientId)
{
  LoginRequest expected("player", "secret", GameRole::GUNNER);
  expected.setClientId(net::ClientId{2});

  const auto cloned = expected.clone();

  ASSERT_EQ(cloned->type(), MessageType::LOGIN_REQUEST);
  assertMessagesAreEqual(cloned->as<LoginRequest>(), expected);
}

} // namespace bsgalone::core
