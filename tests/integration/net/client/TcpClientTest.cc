
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

namespace net::details {
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

} // namespace net::details
