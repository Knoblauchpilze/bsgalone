

#include "MessageParser.hh"
#include "LoginRequest.hh"
#include "SignupRequest.hh"
#include <gtest/gtest.h>

namespace bsgalone::core {
namespace {
template<typename T>
auto serializeMessage(const T &message) -> std::deque<char>
{
  std::stringstream out;
  out << message;
  const auto serialized = out.str();
  return std::deque<char>(serialized.begin(), serialized.end());
}
} // namespace

TEST(Unit_Bsgalone_Core_Messages_Serialization_MessageParser,
     DoesNotProcessBytesWhenMessageIsNotComplete)
{
  MessageParser parser{};

  LoginRequest message("dude", "secret", GameRole::PILOT);
  const auto allBytes = serializeMessage(message);
  ASSERT_LT(6, allBytes.size());
  const std::deque<char> partialBytes(allBytes.begin(), allBytes.begin() + 5);

  const auto maybeResult = parser.tryParseMessage(partialBytes);

  EXPECT_EQ(0, maybeResult.bytesProcessed);
  EXPECT_FALSE(maybeResult.message.has_value());
}

TEST(Unit_Bsgalone_Core_Messages_Serialization_MessageParser,
     DoesNotProcessAllBytesWhenMessageIsAlreadyComplete)
{
  MessageParser parser{};

  LoginRequest loginMessage("dude", "secret", GameRole::PILOT);
  const auto loginBytes = serializeMessage(loginMessage);
  SignupRequest signupMessage("dude", "secret", Faction::CYLON);
  const auto signupBytes = serializeMessage(signupMessage);

  std::deque<char> loginAndMore = loginBytes;
  ASSERT_LT(6, signupBytes.size());
  loginAndMore.insert(loginAndMore.end(), signupBytes.begin(), signupBytes.begin() + 5);

  const auto maybeResult = parser.tryParseMessage(loginAndMore);

  EXPECT_EQ(loginBytes.size(), maybeResult.bytesProcessed);
  EXPECT_TRUE(maybeResult.message.has_value());
  EXPECT_EQ(MessageType::LOGIN_REQUEST, (*maybeResult.message)->type());
}

TEST(Unit_Bsgalone_Core_Messages_Serialization_MessageParser, DeserializesLoginRequest)
{
  MessageParser parser{};

  LoginRequest message("dude", "secret", GameRole::PILOT);
  const auto bytes = serializeMessage(message);

  const auto maybeResult = parser.tryParseMessage(bytes);

  EXPECT_EQ(bytes.size(), maybeResult.bytesProcessed);
  EXPECT_TRUE(maybeResult.message.has_value());
  EXPECT_EQ(MessageType::LOGIN_REQUEST, (*maybeResult.message)->type());
  const auto &actual = (*maybeResult.message)->as<LoginRequest>();
  EXPECT_EQ("dude", actual.getUsername());
  EXPECT_EQ("secret", actual.getPassword());
  EXPECT_EQ(GameRole::PILOT, actual.getRole());
  EXPECT_FALSE(actual.tryGetClientId().has_value());
}

TEST(Unit_Bsgalone_Core_Messages_Serialization_MessageParser, DeserializesLoginRequestWithClientId)
{
  MessageParser parser{};

  LoginRequest message("dude", "secret", GameRole::PILOT);
  message.setClientId(net::ClientId{12});
  const auto bytes = serializeMessage(message);

  const auto maybeResult = parser.tryParseMessage(bytes);

  EXPECT_EQ(bytes.size(), maybeResult.bytesProcessed);
  EXPECT_TRUE(maybeResult.message.has_value());
  EXPECT_EQ(MessageType::LOGIN_REQUEST, (*maybeResult.message)->type());
  const auto &actual = (*maybeResult.message)->as<LoginRequest>();
  EXPECT_EQ("dude", actual.getUsername());
  EXPECT_EQ("secret", actual.getPassword());
  EXPECT_EQ(GameRole::PILOT, actual.getRole());
  EXPECT_EQ(net::ClientId{12}, actual.tryGetClientId().value());
}

TEST(Unit_Bsgalone_Core_Messages_Serialization_MessageParser, DeserializesSignupRequest)
{
  MessageParser parser{};

  SignupRequest message("dude", "secret", Faction::CYLON);
  const auto bytes = serializeMessage(message);

  const auto maybeResult = parser.tryParseMessage(bytes);

  EXPECT_EQ(bytes.size(), maybeResult.bytesProcessed);
  EXPECT_TRUE(maybeResult.message.has_value());
  EXPECT_EQ(MessageType::SIGNUP_REQUEST, (*maybeResult.message)->type());
  const auto &actual = (*maybeResult.message)->as<SignupRequest>();
  EXPECT_EQ("dude", actual.getUsername());
  EXPECT_EQ("secret", actual.getPassword());
  EXPECT_EQ(Faction::CYLON, actual.getFaction());
  EXPECT_FALSE(actual.tryGetClientId().has_value());
}

TEST(Unit_Bsgalone_Core_Messages_Serialization_MessageParser, DeserializesSignupRequestWithClientId)
{
  MessageParser parser{};

  SignupRequest message("dude", "secret", Faction::CYLON);
  message.setClientId(net::ClientId{12});
  const auto bytes = serializeMessage(message);

  const auto maybeResult = parser.tryParseMessage(bytes);

  EXPECT_EQ(bytes.size(), maybeResult.bytesProcessed);
  EXPECT_TRUE(maybeResult.message.has_value());
  EXPECT_EQ(MessageType::SIGNUP_REQUEST, (*maybeResult.message)->type());
  const auto &actual = (*maybeResult.message)->as<SignupRequest>();
  EXPECT_EQ("dude", actual.getUsername());
  EXPECT_EQ("secret", actual.getPassword());
  EXPECT_EQ(Faction::CYLON, actual.getFaction());
  EXPECT_TRUE(actual.tryGetClientId().has_value());
  EXPECT_EQ(net::ClientId{12}, actual.tryGetClientId().value());
}

} // namespace bsgalone::core
