
#include "OutputNetworkAdapter.hh"
#include "LoginRequest.hh"
#include "TestNetworkClient.hh"
#include "TestNetworkServer.hh"
#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace test;
using namespace ::testing;

namespace bsgalone::core {
namespace {
class MockSerializer : public IMessageSerializer
{
  public:
  MockSerializer()           = default;
  ~MockSerializer() override = default;

  MOCK_METHOD(std::vector<char>, serializeMessage, (const IMessage &), (override));
};
} // namespace

TEST(Unit_Bsgalone_Core_Messages_Network_OutputNetworkAdapter, ThrowsWhenServerIsNull)
{
  EXPECT_THROW(
    []() {
      OutputNetworkAdapter(std::shared_ptr<TestNetworkServer>{}, std::make_unique<MockSerializer>());
    }(),
    std::invalid_argument);
}

TEST(Unit_Bsgalone_Core_Messages_Network_OutputNetworkAdapter, ThrowsWhenSerializerIsNullForServer)
{
  EXPECT_THROW([]() { OutputNetworkAdapter(std::make_shared<TestNetworkServer>(), nullptr); }(),
               std::invalid_argument);
}

TEST(Unit_Bsgalone_Core_Messages_Network_OutputNetworkAdapter, ThrowsWhenClientIsNull)
{
  EXPECT_THROW(
    []() {
      OutputNetworkAdapter(std::shared_ptr<TestNetworkClient>{}, std::make_unique<MockSerializer>());
    }(),
    std::invalid_argument);
}

TEST(Unit_Bsgalone_Core_Messages_Network_OutputNetworkAdapter, ThrowsWhenSerializerIsNullForClient)
{
  EXPECT_THROW([]() { OutputNetworkAdapter(std::make_shared<TestNetworkServer>(), nullptr); }(),
               std::invalid_argument);
}

TEST(Unit_Bsgalone_Core_Messages_Network_OutputNetworkAdapter,
     ThrowsWhenSendingAServerMessageWithClientConfigured)
{
  OutputNetworkAdapter adapter(std::make_shared<TestNetworkClient>(),
                               std::make_unique<MockSerializer>());

  EXPECT_THROW(
    [&adapter]() {
      LoginRequest message("player", "password", GameRole::GUNNER);
      adapter.sendMessage(net::ClientId{12}, message);
    }(),
    std::invalid_argument);
}

TEST(Unit_Bsgalone_Core_Messages_Network_OutputNetworkAdapter,
     ThrowsWhenSendingAClientMessageWithServerConfigured)
{
  OutputNetworkAdapter adapter(std::make_shared<TestNetworkServer>(),
                               std::make_unique<MockSerializer>());

  EXPECT_THROW(
    [&adapter]() {
      LoginRequest message("player", "password", GameRole::GUNNER);
      adapter.sendMessage(message);
    }(),
    std::invalid_argument);
}

TEST(Unit_Bsgalone_Core_Messages_Network_OutputNetworkAdapter, SerializeServersMessageToSend)
{
  LoginRequest message("player", "password", GameRole::GUNNER);

  auto serializer = std::make_unique<StrictMock<MockSerializer>>();
  core::IMessagePtr captured;
  // https://stackoverflow.com/questions/46157888/gmock-capture-a-reference-argument-in-a-mocked-function
  EXPECT_CALL(*serializer, serializeMessage(_))
    .Times(1)
    .WillOnce(Invoke([&captured](const core::IMessage &message) {
      captured = message.clone();
      return std::vector<char>{};
    }));

  auto server = std::make_shared<TestNetworkServer>();
  OutputNetworkAdapter adapter(server, std::move(serializer));

  adapter.sendMessage(net::ClientId{12}, message);

  EXPECT_EQ(MessageType::LOGIN_REQUEST, captured->type());
  const auto &actual = captured->as<LoginRequest>();
  EXPECT_EQ("player", actual.getUsername());
  EXPECT_EQ("password", actual.getPassword());
  EXPECT_EQ(GameRole::GUNNER, actual.getRole());
  EXPECT_FALSE(actual.tryGetClientId().has_value());
}

TEST(Unit_Bsgalone_Core_Messages_Network_OutputNetworkAdapter,
     SendsServerMessageWithSerializationResult)
{
  LoginRequest message("player", "password", GameRole::GUNNER);

  // This does not correspond to the serialized version of the request but
  // it does not matter for this test.
  std::vector<char> mockSerialized{'t', 'e', 's', 't'};
  auto serializer = std::make_unique<StrictMock<MockSerializer>>();
  EXPECT_CALL(*serializer, serializeMessage(_)).Times(1).WillOnce(Return(mockSerialized));

  auto server = std::make_shared<TestNetworkServer>();
  OutputNetworkAdapter adapter(server, std::move(serializer));

  adapter.sendMessage(net::ClientId{12}, message);

  EXPECT_EQ(1u, server->messages().size());
  const auto &actual = server->messages().at(0);
  EXPECT_EQ(net::ClientId{12}, actual.clientId);
  EXPECT_EQ(mockSerialized, actual.data);
}

TEST(Unit_Bsgalone_Core_Messages_Network_OutputNetworkAdapter, SerializeClientMessageToSend)
{
  LoginRequest message("player", "password", GameRole::GUNNER);

  auto serializer = std::make_unique<StrictMock<MockSerializer>>();
  core::IMessagePtr captured;
  EXPECT_CALL(*serializer, serializeMessage(_))
    .Times(1)
    .WillOnce(Invoke([&captured](const core::IMessage &message) {
      captured = message.clone();
      return std::vector<char>{};
    }));

  auto client = std::make_shared<TestNetworkClient>();
  OutputNetworkAdapter adapter(client, std::move(serializer));

  adapter.sendMessage(message);

  EXPECT_EQ(MessageType::LOGIN_REQUEST, captured->type());
  const auto &actual = captured->as<LoginRequest>();
  EXPECT_EQ("player", actual.getUsername());
  EXPECT_EQ("password", actual.getPassword());
  EXPECT_EQ(GameRole::GUNNER, actual.getRole());
  EXPECT_FALSE(actual.tryGetClientId().has_value());
}

TEST(Unit_Bsgalone_Core_Messages_Network_OutputNetworkAdapter,
     SendsClientMessageWithSerializationResult)
{
  LoginRequest message("player", "password", GameRole::GUNNER);

  // This does not correspond to the serialized version of the request but
  // it does not matter for this test.
  std::vector<char> mockSerialized{'t', 'e', 's', 't'};
  auto serializer = std::make_unique<StrictMock<MockSerializer>>();
  EXPECT_CALL(*serializer, serializeMessage(_)).Times(1).WillOnce(Return(mockSerialized));

  auto client = std::make_shared<TestNetworkClient>();
  OutputNetworkAdapter adapter(client, std::move(serializer));

  adapter.sendMessage(message);

  EXPECT_EQ(1u, client->messages().size());
  const auto &actual = client->messages().at(0);
  EXPECT_EQ(mockSerialized, actual.data);
}

} // namespace bsgalone::core
