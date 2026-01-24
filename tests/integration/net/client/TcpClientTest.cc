
#include "TcpClient.hh"
#include "ClientConnectedEvent.hh"
#include "ClientDisconnectedEvent.hh"
#include "ConnectedSockets.hh"
#include "DataReceivedEvent.hh"
#include "DataSentEvent.hh"
#include "TcpServerFixture.hh"
#include "TestEventBus.hh"
#include <gtest/gtest.h>

using namespace test;
using namespace ::testing;

namespace net {
using Integration_Net_Client_TcpClient = TcpServerFixture;

constexpr auto LOCALHOST_URL = "127.0.0.1";

TEST_F(Integration_Net_Client_TcpClient, ThrowsWhenEventBusIsNull)
{
  EXPECT_THROW([this]() { TcpClient(nullptr); }(), std::invalid_argument);
}

TEST_F(Integration_Net_Client_TcpClient, ThrowsWhenConnectingMultipleTimes)
{
  auto bus    = std::make_shared<TestEventBus>();
  auto client = std::make_shared<TcpClient>(bus);
  client->connect(LOCALHOST_URL, this->port());

  auto body = [this, &client]() { client->connect(LOCALHOST_URL, this->port()); };

  EXPECT_THROW(body(), core::CoreException);
}

TEST_F(Integration_Net_Client_TcpClient, ThrowsWhenDisconnectingWithoutConnecting)
{
  auto bus    = std::make_shared<TestEventBus>();
  auto client = std::make_shared<TcpClient>(bus);

  EXPECT_THROW([&client]() { client->disconnect(); }(), core::CoreException);
}

TEST_F(Integration_Net_Client_TcpClient, EstablishesConnectionAndPublishesClientConnectedEvent)
{
  auto bus    = std::make_shared<TestEventBus>();
  auto client = std::make_shared<TcpClient>(bus);
  client->connect(LOCALHOST_URL, this->port());

  auto sockets = this->waitForServerSocket();
  sockets.writeServer(ClientId{34});

  const auto actual = bus->waitForEvent();
  EXPECT_EQ(EventType::CLIENT_CONNECTED, actual->type());
  EXPECT_EQ(ClientId{34}, actual->as<ClientConnectedEvent>().clientId());
}

TEST_F(Integration_Net_Client_TcpClient, DetectsDisconnectionAndPublishesClientDisconnectedEvent)
{
  auto bus    = std::make_shared<TestEventBus>();
  auto client = std::make_shared<TcpClient>(bus);
  client->connect(LOCALHOST_URL, this->port());

  auto sockets = this->waitForServerSocket();
  sockets.writeServer(ClientId{34});

  auto event = bus->waitForEvent();
  EXPECT_EQ(EventType::CLIENT_CONNECTED, event->type());
  EXPECT_EQ(ClientId{34}, event->as<ClientConnectedEvent>().clientId());

  sockets.server->shutdown(asio::ip::tcp::socket::shutdown_both);
  event = bus->waitForEvent();
  EXPECT_EQ(EventType::CLIENT_DISCONNECTED, event->type());
  EXPECT_EQ(ClientId{34}, event->as<ClientDisconnectedEvent>().clientId());
}

TEST_F(Integration_Net_Client_TcpClient, PublishesClientDisconnectedEventWhenClientDisconnects)
{
  auto bus    = std::make_shared<TestEventBus>();
  auto client = std::make_shared<TcpClient>(bus);
  client->connect(LOCALHOST_URL, this->port());

  auto sockets = this->waitForServerSocket();
  sockets.writeServer(ClientId{34});

  auto event = bus->waitForEvent();
  EXPECT_EQ(EventType::CLIENT_CONNECTED, event->type());
  EXPECT_EQ(ClientId{34}, event->as<ClientConnectedEvent>().clientId());

  client->disconnect();

  event = bus->waitForEvent();
  EXPECT_EQ(EventType::CLIENT_DISCONNECTED, event->type());
  EXPECT_EQ(ClientId{34}, event->as<ClientDisconnectedEvent>().clientId());
}

TEST_F(Integration_Net_Client_TcpClient, SendsDataToServerSocket)
{
  auto bus    = std::make_shared<TestEventBus>();
  auto client = std::make_shared<TcpClient>(bus);
  client->connect(LOCALHOST_URL, this->port());

  auto sockets = this->waitForServerSocket();
  sockets.writeServer(ClientId{34});

  auto event = bus->waitForEvent();
  EXPECT_EQ(EventType::CLIENT_CONNECTED, event->type());
  EXPECT_EQ(ClientId{34}, event->as<ClientConnectedEvent>().clientId());

  std::string data("test");
  client->trySend(std::vector<char>(data.begin(), data.end()));

  const auto actual = sockets.readServer(data.size());
  EXPECT_EQ("test", actual);
}

TEST_F(Integration_Net_Client_TcpClient, PublishesDataSentEvent)
{
  auto bus    = std::make_shared<TestEventBus>();
  auto client = std::make_shared<TcpClient>(bus);
  client->connect(LOCALHOST_URL, this->port());

  auto sockets = this->waitForServerSocket();
  sockets.writeServer(ClientId{34});

  auto event = bus->waitForEvent();
  EXPECT_EQ(EventType::CLIENT_CONNECTED, event->type());
  EXPECT_EQ(ClientId{34}, event->as<ClientConnectedEvent>().clientId());

  std::string data("test");
  const auto expectedMessageId = client->trySend(std::vector<char>(data.begin(), data.end()));
  EXPECT_TRUE(expectedMessageId.has_value());

  event = bus->waitForEvent();
  EXPECT_EQ(EventType::DATA_SENT, event->type());
  EXPECT_EQ(ClientId{34}, event->as<DataSentEvent>().clientId());
  const std::vector<char> expectedData(data.begin(), data.end());
  EXPECT_EQ(expectedMessageId.value(), event->as<DataSentEvent>().messageId());
}

} // namespace net
