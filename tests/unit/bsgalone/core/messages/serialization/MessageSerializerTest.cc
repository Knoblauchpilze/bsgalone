

#include "MessageSerializer.hh"
#include "LoginMessage.hh"
#include "LoginRequest.hh"
#include "LogoutMessage.hh"
#include "LogoutRequest.hh"
#include "SignupMessage.hh"
#include "SignupRequest.hh"
#include "UndockMessage.hh"
#include "UndockRequest.hh"
#include <gtest/gtest.h>

namespace bsgalone::core {

TEST(Unit_Bsgalone_Core_Messages_Serialization_MessageSerializer, SerializesLoginMessage)
{
  MessageSerializer serializer{};

  LoginMessage message;
  message.setPlayerDbId(Uuid{});
  message.setFaction(Faction::COLONIAL);
  message.setRole(GameRole::PILOT);

  const auto actual = serializer.serializeMessage(message);

  std::stringstream out;
  out << message;
  const auto serialized = out.str();
  std::vector<char> expected(serialized.begin(), serialized.end());
  EXPECT_EQ(expected, actual);
}

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

TEST(Unit_Bsgalone_Core_Messages_Serialization_MessageSerializer, SerializesLogoutMessage)
{
  MessageSerializer serializer{};

  LogoutMessage message(Uuid{});

  const auto actual = serializer.serializeMessage(message);

  std::stringstream out;
  out << message;
  const auto serialized = out.str();
  std::vector<char> expected(serialized.begin(), serialized.end());
  EXPECT_EQ(expected, actual);
}

TEST(Unit_Bsgalone_Core_Messages_Serialization_MessageSerializer, SerializesLogoutRequest)
{
  MessageSerializer serializer{};

  LogoutRequest message(Uuid{});

  const auto actual = serializer.serializeMessage(message);

  std::stringstream out;
  out << message;
  const auto serialized = out.str();
  std::vector<char> expected(serialized.begin(), serialized.end());
  EXPECT_EQ(expected, actual);
}

TEST(Unit_Bsgalone_Core_Messages_Serialization_MessageSerializer, SerializesSignupMessage)
{
  MessageSerializer serializer{};

  SignupMessage message(false);

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

TEST(Unit_Bsgalone_Core_Messages_Serialization_MessageSerializer, SerializesUndockMessage)
{
  MessageSerializer serializer{};

  UndockMessage message(Uuid{});

  const auto actual = serializer.serializeMessage(message);

  std::stringstream out;
  out << message;
  const auto serialized = out.str();
  std::vector<char> expected(serialized.begin(), serialized.end());
  EXPECT_EQ(expected, actual);
}

TEST(Unit_Bsgalone_Core_Messages_Serialization_MessageSerializer, SerializesUndockRequest)
{
  MessageSerializer serializer{};

  UndockRequest message(Uuid{});

  const auto actual = serializer.serializeMessage(message);

  std::stringstream out;
  out << message;
  const auto serialized = out.str();
  std::vector<char> expected(serialized.begin(), serialized.end());
  EXPECT_EQ(expected, actual);
}

} // namespace bsgalone::core
