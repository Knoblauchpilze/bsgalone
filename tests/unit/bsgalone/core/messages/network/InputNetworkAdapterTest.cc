
#include "InputNetworkAdapter.hh"
#include "DataReceivedEvent.hh"
#include "LoginRequest.hh"
#include "MessageParser.hh"
#include "SignupRequest.hh"
#include "TestMessageQueue.hh"
#include <gtest/gtest.h>

using namespace test;
using namespace ::testing;

namespace bsgalone::core {
namespace {
auto generateSerializedTestMessage() -> std::vector<char>
{
  LoginRequest message("name", "secret", GameRole::PILOT);
  std::stringstream out;
  out << message;
  const auto serialized = out.str();
  return std::vector<char>(serialized.begin(), serialized.end());
}
} // namespace

TEST(Unit_Bsgalone_Core_Messages_Network_InputNetworkAdapter, ThrowsWhenEventQueueIsNull)
{
  EXPECT_THROW([]() { InputNetworkAdapter(nullptr, std::make_unique<MessageParser>()); }(),
               std::invalid_argument);
}

TEST(Unit_Bsgalone_Core_Messages_Network_InputNetworkAdapter, ThrowsWhenMessageParserIsNull)
{
  EXPECT_THROW([]() { InputNetworkAdapter(std::make_shared<TestMessageQueue>(), nullptr); }(),
               std::invalid_argument);
}

TEST(Unit_Bsgalone_Core_Messages_Network_InputNetworkAdapter, CorrectlyPublishesCompleteMessage)
{
  auto queue = std::make_shared<TestMessageQueue>();
  InputNetworkAdapter adapter(queue, std::make_unique<MessageParser>());

  const auto data = generateSerializedTestMessage();
  const net::DataReceivedEvent event(net::ClientId{1}, data);

  adapter.onEventReceived(event);

  EXPECT_EQ(1u, queue->messages().size());
  EXPECT_EQ(MessageType::LOGIN_REQUEST, queue->messages().at(0)->type());
  const auto &actual = queue->messages().at(0)->as<LoginRequest>();
  EXPECT_EQ(net::ClientId{1}, actual.tryGetClientId().value());
  EXPECT_EQ("name", actual.getUsername());
  EXPECT_EQ("secret", actual.getPassword());
  EXPECT_EQ(GameRole::PILOT, actual.getRole());
}

// TODO: Restore a test when a message exists with no client identifier.

TEST(Unit_Bsgalone_Core_Messages_Network_InputNetworkAdapter,
     DoesNotPublishMessageWhenIncompleteDataIsReceived)
{
  auto queue = std::make_shared<TestMessageQueue>();
  InputNetworkAdapter adapter(queue, std::make_unique<MessageParser>());

  const auto data = generateSerializedTestMessage();
  ASSERT_LE(6, data.size());
  const net::DataReceivedEvent event(net::ClientId{0},
                                     std::vector<char>(data.begin(), data.begin() + 5));

  adapter.onEventReceived(event);

  EXPECT_TRUE(queue->empty());
}

TEST(Unit_Bsgalone_Core_Messages_Network_InputNetworkAdapter, PublishesMessageWhenAllDataIsReceived)
{
  auto queue = std::make_shared<TestMessageQueue>();
  InputNetworkAdapter adapter(queue, std::make_unique<MessageParser>());

  const auto data = generateSerializedTestMessage();
  ASSERT_LE(6, data.size());
  net::DataReceivedEvent event(net::ClientId{0}, std::vector<char>(data.begin(), data.begin() + 5));

  adapter.onEventReceived(event);
  EXPECT_TRUE(queue->empty());

  event = net::DataReceivedEvent(net::ClientId{0}, std::vector<char>(data.begin() + 5, data.end()));
  adapter.onEventReceived(event);

  EXPECT_EQ(1u, queue->messages().size());
  EXPECT_EQ(MessageType::LOGIN_REQUEST, queue->messages().at(0)->type());
  const auto &actual = queue->messages().at(0)->as<LoginRequest>();
  EXPECT_EQ(net::ClientId{0}, actual.tryGetClientId().value());
  EXPECT_EQ("name", actual.getUsername());
  EXPECT_EQ("secret", actual.getPassword());
  EXPECT_EQ(GameRole::PILOT, actual.getRole());
}

TEST(Unit_Bsgalone_Core_Messages_Network_InputNetworkAdapter, DoesNotPublishMessageTwice)
{
  auto queue = std::make_shared<TestMessageQueue>();
  InputNetworkAdapter adapter(queue, std::make_unique<MessageParser>());

  const auto data = generateSerializedTestMessage();
  net::DataReceivedEvent event(net::ClientId{0}, data);

  adapter.onEventReceived(event);
  EXPECT_FALSE(queue->empty());

  queue->clearMessages();

  event = net::DataReceivedEvent(net::ClientId{0}, std::vector<char>());
  adapter.onEventReceived(event);

  EXPECT_TRUE(queue->empty());
}

TEST(Unit_Bsgalone_Core_Messages_Network_InputNetworkAdapter,
     DoesNotPublishMessageWhenDataReceivedForAnotherClient)
{
  auto queue = std::make_shared<TestMessageQueue>();
  InputNetworkAdapter adapter(queue, std::make_unique<MessageParser>());

  const auto data = generateSerializedTestMessage();
  ASSERT_LE(6, data.size());
  net::DataReceivedEvent event(net::ClientId{0}, std::vector<char>(data.begin(), data.begin() + 5));

  adapter.onEventReceived(event);
  EXPECT_TRUE(queue->empty());

  event = net::DataReceivedEvent(net::ClientId{1},
                                 std::vector<char>(data.begin(), data.begin() + 5));
  adapter.onEventReceived(event);

  EXPECT_TRUE(queue->empty());
}

TEST(Unit_Bsgalone_Core_Messages_Network_InputNetworkAdapter,
     DoesNotPublishMessageWhenDataReceivedForNoClient)
{
  auto queue = std::make_shared<TestMessageQueue>();
  InputNetworkAdapter adapter(queue, std::make_unique<MessageParser>());

  const auto data = generateSerializedTestMessage();
  ASSERT_LE(6, data.size());
  net::DataReceivedEvent event(net::ClientId{0}, std::vector<char>(data.begin(), data.begin() + 5));

  adapter.onEventReceived(event);
  EXPECT_TRUE(queue->empty());

  event = net::DataReceivedEvent({}, std::vector<char>(data.begin(), data.begin() + 5));
  adapter.onEventReceived(event);

  EXPECT_TRUE(queue->empty());
}

TEST(Unit_Bsgalone_Core_Messages_Network_InputNetworkAdapter, OnlyPublishMessageForReceivedData)
{
  auto queue = std::make_shared<TestMessageQueue>();
  InputNetworkAdapter adapter(queue, std::make_unique<MessageParser>());

  const auto data = generateSerializedTestMessage();
  ASSERT_LE(6, data.size());

  net::DataReceivedEvent event(net::ClientId{0}, std::vector<char>(data.begin(), data.begin() + 5));
  adapter.onEventReceived(event);
  EXPECT_TRUE(queue->empty());

  event = net::DataReceivedEvent(net::ClientId{1},
                                 std::vector<char>(data.begin(), data.begin() + 5));
  adapter.onEventReceived(event);
  EXPECT_TRUE(queue->empty());

  event = net::DataReceivedEvent(net::ClientId{0}, std::vector<char>(data.begin() + 5, data.end()));
  adapter.onEventReceived(event);

  EXPECT_EQ(1u, queue->messages().size());
  EXPECT_EQ(MessageType::LOGIN_REQUEST, queue->messages().at(0)->type());
  const auto &actual = queue->messages().at(0)->as<LoginRequest>();
  EXPECT_EQ(net::ClientId{0}, actual.tryGetClientId().value());
  EXPECT_EQ("name", actual.getUsername());
  EXPECT_EQ("secret", actual.getPassword());
  EXPECT_EQ(GameRole::PILOT, actual.getRole());
}

TEST(Unit_Bsgalone_Core_Messages_Network_InputNetworkAdapter, ThrowsWhenReceivingInvalidData)
{
  auto queue = std::make_shared<TestMessageQueue>();
  InputNetworkAdapter adapter(queue, std::make_unique<MessageParser>());

  const auto data = generateSerializedTestMessage();
  ASSERT_LE(6, data.size());
  net::DataReceivedEvent event(net::ClientId{0}, std::vector<char>(data.begin() + 4, data.end()));

  auto body = [&adapter, &event]() { adapter.onEventReceived(event); };
  EXPECT_THROW(body(), ::core::CoreException);
}

TEST(Unit_Bsgalone_Core_Messages_Network_InputNetworkAdapter,
     SetsClientIdForLoginRequestWhenNotDefined)
{
  auto queue = std::make_shared<TestMessageQueue>();
  InputNetworkAdapter adapter(queue, std::make_unique<MessageParser>());

  LoginRequest message("player", "password", GameRole::GUNNER);
  std::stringstream out;
  out << message;
  const auto serialized = out.str();
  std::vector<char> data(serialized.begin(), serialized.end());

  net::DataReceivedEvent event(net::ClientId{17}, std::vector<char>(data.begin(), data.end()));
  adapter.onEventReceived(event);

  EXPECT_EQ(1u, queue->messages().size());
  const auto &actual = queue->messages().at(0);
  EXPECT_EQ(MessageType::LOGIN_REQUEST, actual->type());
  EXPECT_EQ(net::ClientId{17}, actual->as<LoginRequest>().tryGetClientId().value());
}

TEST(Unit_Bsgalone_Core_Messages_Network_InputNetworkAdapter,
     SetsClientIdForSignupRequestWhenNotDefined)
{
  auto queue = std::make_shared<TestMessageQueue>();
  InputNetworkAdapter adapter(queue, std::make_unique<MessageParser>());

  SignupRequest message("player", "password", Faction::COLONIAL);
  std::stringstream out;
  out << message;
  const auto serialized = out.str();
  std::vector<char> data(serialized.begin(), serialized.end());

  net::DataReceivedEvent event(net::ClientId{18}, std::vector<char>(data.begin(), data.end()));
  adapter.onEventReceived(event);

  EXPECT_EQ(1u, queue->messages().size());
  const auto &actual = queue->messages().at(0);
  EXPECT_EQ(MessageType::SIGNUP_REQUEST, actual->type());
  EXPECT_EQ(net::ClientId{18}, actual->as<SignupRequest>().tryGetClientId().value());
}

TEST(Unit_Bsgalone_Core_Messages_Network_InputNetworkAdapter,
     OverridesExistingClientIdWithConnectionData)
{
  auto queue = std::make_shared<TestMessageQueue>();
  InputNetworkAdapter adapter(queue, std::make_unique<MessageParser>());

  SignupRequest message("player", "password", Faction::COLONIAL);
  message.setClientId(net::ClientId{19});
  std::stringstream out;
  out << message;
  const auto serialized = out.str();
  std::vector<char> data(serialized.begin(), serialized.end());

  net::DataReceivedEvent event(net::ClientId{17}, std::vector<char>(data.begin(), data.end()));
  adapter.onEventReceived(event);

  EXPECT_EQ(1u, queue->messages().size());
  const auto &actual = queue->messages().at(0);
  EXPECT_EQ(MessageType::SIGNUP_REQUEST, actual->type());
  EXPECT_EQ(net::ClientId{17}, actual->as<SignupRequest>().tryGetClientId().value());
}

} // namespace bsgalone::core
