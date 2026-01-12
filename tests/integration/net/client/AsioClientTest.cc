
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
using Integration_Net_Server_AsioClient = TcpServerFixture;

constexpr auto LOCALHOST_URL = "127.0.0.1";

TEST_F(Integration_Net_Server_AsioClient, ThrowsWhenEventBusIsNull)
{
  EXPECT_THROW([this]() { AsioClient(nullptr); }(), std::invalid_argument);
}

TEST_F(Integration_Net_Server_AsioClient, ConnectsToServerAndPublishesClientConnectedEvent)
{
  auto bus    = std::make_shared<TestEventBus>();
  auto client = std::make_shared<AsioClient>(bus);
  client->connect(this->asioContext(), LOCALHOST_URL, this->port());

  auto sockets = this->waitForServerSocket();
  sockets.writeServer(ClientId{255});

  const auto actual = bus->waitForEvent();
  EXPECT_EQ(EventType::CLIENT_CONNECTED, actual->type());
  EXPECT_EQ(ClientId{255}, actual->as<ClientConnectedEvent>().clientId());
}

TEST_F(Integration_Net_Server_AsioClient, DetectsDisconnectionAndPublishesClientDisconnectedEvent)
{
  auto bus    = std::make_shared<TestEventBus>();
  auto client = std::make_shared<AsioClient>(bus);
  client->connect(this->asioContext(), LOCALHOST_URL, this->port());

  auto sockets = this->waitForServerSocket();
  sockets.writeServer(ClientId{32});
  auto event = bus->waitForEvent();
  EXPECT_EQ(EventType::CLIENT_CONNECTED, event->type());
  const auto expectedClientId = event->as<ClientConnectedEvent>().clientId();

  sockets.server->shutdown(asio::ip::tcp::socket::shutdown_both);
  event = bus->waitForEvent();
  EXPECT_EQ(EventType::CLIENT_DISCONNECTED, event->type());
  EXPECT_EQ(expectedClientId, event->as<ClientDisconnectedEvent>().clientId());
}

TEST_F(Integration_Net_Server_AsioClient, PublishesDataReceivedEventWhenDataIsReceived)
{
  auto bus    = std::make_shared<TestEventBus>();
  auto client = std::make_shared<AsioClient>(bus);
  client->connect(this->asioContext(), LOCALHOST_URL, this->port());

  auto sockets = this->waitForServerSocket();
  sockets.writeServer(ClientId{32});
  auto event = bus->waitForEvent();
  EXPECT_EQ(EventType::CLIENT_CONNECTED, event->type());
  const auto expectedClientId = event->as<ClientConnectedEvent>().clientId();

  std::string data("test");
  sockets.writeServer(data);

  event = bus->waitForEvent();
  EXPECT_EQ(EventType::DATA_RECEIVED, event->type());
  EXPECT_EQ(expectedClientId, event->as<DataReceivedEvent>().clientId());
  const std::vector<char> expectedData(data.begin(), data.end());
  EXPECT_EQ(expectedData, event->as<DataReceivedEvent>().data());
}

// TODO: This one hangs because although the client is deleted (see below) the sockets
// are still there and do not send the message.
// To solve the issue we could probably shutdown the sockets in the destructor of the
// client. Although the sockets are now sending the event nobody relays it to the event
// bus and it's still hanging
// Maybe what could work would be to have the proxy directly forward messages by having
// a pointer to the external event bus
// The client can then only be interested in the data read/write failure
// This would still not solve the problem of the disconnection because those are handled
// by the socket.
// Two ways to go about it:
//  - either the client does not own the socket but the proxy does
//  - the client should somehow live as long as the sockets
TEST_F(Integration_Net_Server_AsioClient, PublishesClientDisconnectedEventWhenClientSocketAreDeleted)
{
  auto bus = std::make_shared<TestEventBus>();

  std::optional<ClientId> expectedClientId{};

  // Note: see also the same test in Integration_Net_Server_AsioServer
  {
    auto client = std::make_shared<AsioClient>(bus);
    client->connect(this->asioContext(), LOCALHOST_URL, this->port());

    auto sockets = this->waitForServerSocket();
    sockets.writeServer(ClientId{32});
    auto event = bus->waitForEvent();
    EXPECT_EQ(EventType::CLIENT_CONNECTED, event->type());
    expectedClientId = event->as<ClientConnectedEvent>().clientId();
  }

  auto event = bus->waitForEvent();
  EXPECT_EQ(EventType::CLIENT_DISCONNECTED, event->type());
  ASSERT_TRUE(expectedClientId.has_value());
  EXPECT_EQ(expectedClientId, event->as<ClientDisconnectedEvent>().clientId());
}

TEST_F(Integration_Net_Server_AsioClient, ThrowsWhenWritingAMessageAndNotConnected)
{
  auto bus    = std::make_shared<TestEventBus>();
  auto client = std::make_shared<AsioClient>(bus);

  EXPECT_THROW([&client]() { client->trySend(std::vector<char>(2, 0)); }(), core::CoreException);
}

TEST_F(Integration_Net_Server_AsioClient, ReturnsEmptyMessageIdentifierWhenMessageIsEmpty)
{
  auto bus    = std::make_shared<TestEventBus>();
  auto client = std::make_shared<AsioClient>(bus);
  client->connect(this->asioContext(), LOCALHOST_URL, this->port());

  auto sockets = this->waitForServerSocket();
  sockets.writeServer(ClientId{32});
  auto event = bus->waitForEvent();
  EXPECT_EQ(EventType::CLIENT_CONNECTED, event->type());

  const auto actualId = client->trySend(std::vector<char>{});

  EXPECT_FALSE(actualId.has_value());
}

TEST_F(Integration_Net_Server_AsioClient, WritesDataToServerSocket)
{
  auto bus    = std::make_shared<TestEventBus>();
  auto client = std::make_shared<AsioClient>(bus);
  client->connect(this->asioContext(), LOCALHOST_URL, this->port());

  auto sockets = this->waitForServerSocket();
  sockets.writeServer(ClientId{32});
  auto event = bus->waitForEvent();
  EXPECT_EQ(EventType::CLIENT_CONNECTED, event->type());

  std::string data("test");
  client->trySend(std::vector<char>(data.begin(), data.end()));

  const auto actual = sockets.readServer(data.size());
  EXPECT_EQ("test", actual);
}

TEST_F(Integration_Net_Server_AsioClient, PublishesDataSentEvent)
{
  auto bus    = std::make_shared<TestEventBus>();
  auto client = std::make_shared<AsioClient>(bus);
  client->connect(this->asioContext(), LOCALHOST_URL, this->port());

  std::cout << "[test] waiting for server socket\n";
  auto sockets = this->waitForServerSocket();
  sockets.writeServer(ClientId{32});
  std::cout << "[test] waiting for client connected\n";
  auto event = bus->waitForEvent();
  EXPECT_EQ(EventType::CLIENT_CONNECTED, event->type());
  const auto expectedClientId = event->as<ClientConnectedEvent>().clientId();

  std::cout << "[test] sending message\n";
  std::string data("test");
  const auto expectedMessageId = client->trySend(std::vector<char>(data.begin(), data.end()));
  EXPECT_TRUE(expectedMessageId.has_value());

  std::cout << "[test] waiting for data sent\n";
  event = bus->waitForEvent();
  EXPECT_EQ(EventType::DATA_SENT, event->type());
  EXPECT_EQ(expectedClientId, event->as<DataSentEvent>().clientId());
  const std::vector<char> expectedData(data.begin(), data.end());
  EXPECT_EQ(expectedMessageId.value(), event->as<DataSentEvent>().messageId());
  std::cout << "[test] end of test\n";
}

} // namespace net::details
