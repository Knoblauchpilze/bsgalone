
#include "AsioClient.hh"
#include "ClientConnectedEvent.hh"
#include "ClientDisconnectedEvent.hh"
#include "DataReceivedEvent.hh"
#include "DataSentEvent.hh"
#include "TcpServerFixture.hh"
#include "TestEventBus.hh"
#include <gtest/gtest.h>

using namespace test;
using namespace ::testing;

namespace net::details {
using Integration_Net_Client_AsioClient = TcpServerFixture;

constexpr auto LOCALHOST_URL = "127.0.0.1";

TEST_F(Integration_Net_Client_AsioClient, ThrowsWhenEventBusIsNull)
{
  EXPECT_THROW([this]() { AsioClient(nullptr); }(), std::invalid_argument);
}

TEST_F(Integration_Net_Client_AsioClient, ConnectsToServerAndPublishesClientConnectedEvent)
{
  auto bus    = std::make_shared<TestEventBus>();
  auto client = std::make_shared<AsioClient>(bus);
  client->connect(this->asioContext(), LOCALHOST_URL, this->port());

  const auto sockets = this->waitForServerSocket();

  const auto actual = bus->waitForEvent();
  EXPECT_EQ(EventType::CLIENT_CONNECTED, actual->type());
  // TODO: This should not contain any client id.
  EXPECT_EQ(net::ClientId{0}, actual->as<ClientConnectedEvent>().clientId());
}

TEST_F(Integration_Net_Client_AsioClient, DetectsDisconnectionAndPublishesClientDisconnectedEvent)
{
  auto bus    = std::make_shared<TestEventBus>();
  auto client = std::make_shared<AsioClient>(bus);
  client->connect(this->asioContext(), LOCALHOST_URL, this->port());

  const auto [expectedClientId, sockets] = this->waitForClientConnectedEvent(bus);

  sockets.server->shutdown(asio::ip::tcp::socket::shutdown_both);
  const auto event = bus->waitForEvent();
  EXPECT_EQ(EventType::CLIENT_DISCONNECTED, event->type());
  EXPECT_EQ(expectedClientId, event->as<ClientDisconnectedEvent>().clientId());
}

TEST_F(Integration_Net_Client_AsioClient, PublishesDataReceivedEventWhenDataIsReceived)
{
  auto bus    = std::make_shared<TestEventBus>();
  auto client = std::make_shared<AsioClient>(bus);
  client->connect(this->asioContext(), LOCALHOST_URL, this->port());

  const auto [expectedClientId, sockets] = this->waitForClientConnectedEvent(bus);

  std::string data("test");
  sockets.writeServer(data);

  const auto event = bus->waitForEvent();
  EXPECT_EQ(EventType::DATA_RECEIVED, event->type());
  EXPECT_EQ(expectedClientId, event->as<DataReceivedEvent>().clientId());
  const std::vector<char> expectedData(data.begin(), data.end());
  EXPECT_EQ(expectedData, event->as<DataReceivedEvent>().data());
}

TEST_F(Integration_Net_Client_AsioClient, PublishesClientDisconnectedEventWhenDisconnectIsCalled)
{
  auto bus = std::make_shared<TestEventBus>();

  auto client = std::make_shared<AsioClient>(bus);
  client->connect(this->asioContext(), LOCALHOST_URL, this->port());

  const auto [expectedClientId, _] = this->waitForClientConnectedEvent(bus);

  client->disconnect();

  const auto event = bus->waitForEvent();
  EXPECT_EQ(EventType::CLIENT_DISCONNECTED, event->type());
  EXPECT_EQ(expectedClientId, event->as<ClientDisconnectedEvent>().clientId());
}

TEST_F(Integration_Net_Client_AsioClient, SucceedsWhenDisconnectingAndNotConnected)
{
  auto bus    = std::make_shared<TestEventBus>();
  auto client = std::make_shared<AsioClient>(bus);

  client->disconnect();
}

TEST_F(Integration_Net_Client_AsioClient, ThrowsWhenWritingAMessageAndNotConnected)
{
  auto bus    = std::make_shared<TestEventBus>();
  auto client = std::make_shared<AsioClient>(bus);

  EXPECT_THROW([&client]() { client->trySend(std::vector<char>(2, 0)); }(), core::CoreException);
}

TEST_F(Integration_Net_Client_AsioClient, ReturnsEmptyMessageIdentifierWhenMessageIsEmpty)
{
  auto bus    = std::make_shared<TestEventBus>();
  auto client = std::make_shared<AsioClient>(bus);
  client->connect(this->asioContext(), LOCALHOST_URL, this->port());

  const auto [expectedClientId, _] = this->waitForClientConnectedEvent(bus);

  const auto actualId = client->trySend(std::vector<char>{});

  EXPECT_FALSE(actualId.has_value());
}

TEST_F(Integration_Net_Client_AsioClient, WritesDataToServerSocket)
{
  auto bus    = std::make_shared<TestEventBus>();
  auto client = std::make_shared<AsioClient>(bus);
  client->connect(this->asioContext(), LOCALHOST_URL, this->port());

  const auto [expectedClientId, sockets] = this->waitForClientConnectedEvent(bus);

  std::string data("test");
  client->trySend(std::vector<char>(data.begin(), data.end()));

  const auto actual = sockets.readServer(data.size());
  EXPECT_EQ("test", actual);
}

TEST_F(Integration_Net_Client_AsioClient, PublishesDataSentEvent)
{
  auto bus    = std::make_shared<TestEventBus>();
  auto client = std::make_shared<AsioClient>(bus);
  client->connect(this->asioContext(), LOCALHOST_URL, this->port());

  const auto [expectedClientId, _] = this->waitForClientConnectedEvent(bus);

  std::string data("test");
  const auto expectedMessageId = client->trySend(std::vector<char>(data.begin(), data.end()));
  EXPECT_TRUE(expectedMessageId.has_value());

  const auto event = bus->waitForEvent();
  EXPECT_EQ(EventType::DATA_SENT, event->type());
  EXPECT_EQ(expectedClientId, event->as<DataSentEvent>().clientId());
  const std::vector<char> expectedData(data.begin(), data.end());
  EXPECT_EQ(expectedMessageId.value(), event->as<DataSentEvent>().messageId());
}

} // namespace net::details
