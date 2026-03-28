

#include "MessageSerializer.hh"
#include "LoginRequest.hh"
#include "SignupRequest.hh"
#include <gtest/gtest.h>

namespace bsgalone::core {

TEST(Unit_Bsgalone_Core_Messages_Serialization_MessageSerializer, SerializesLoginRequest)
{
  MessageSerializer serializer{};

  LoginRequest message("player", "pwd", GameRole::PILOT);

  const auto actual = serializer.serializeMessage(message);

  std::stringstream out;
  out << message;
  const auto serialized = out.str();
  std::vector<char> expected(serialized.begin(), serialized.end());
  EXPECT_EQ(expected, actual);
}

TEST(Unit_Bsgalone_Core_Messages_Serialization_MessageSerializer, SerializesSignupRequest)
{
  MessageSerializer serializer{};

  SignupRequest message("player", "pwd", Faction::CYLON);

  const auto actual = serializer.serializeMessage(message);

  std::stringstream out;
  out << message;
  const auto serialized = out.str();
  std::vector<char> expected(serialized.begin(), serialized.end());
  EXPECT_EQ(expected, actual);
}

} // namespace bsgalone::core
