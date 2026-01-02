
#include "AsioServer.hh"
#include "ClientConnectedEvent.hh"
#include "ClientDisconnectedEvent.hh"
#include "DataReceivedEvent.hh"
#include "TcpFixture.hh"
#include "TestEventBus.hh"
#include <gtest/gtest.h>

using namespace test;
using namespace ::testing;

namespace net::details {
using Integration_Net_Server_AsioServer = TcpFixture;

TEST_F(Integration_Net_Server_AsioServer, ThrowsWhenEventBusIsNull)
{
  EXPECT_THROW([this]() { AsioServer(this->port(), nullptr); }(), std::invalid_argument);
}

TEST_F(Integration_Net_Server_AsioServer, AcceptsConnectionAndPublishesClientConnectedEvent)
{
  auto bus    = std::make_shared<TestEventBus>();
  auto server = std::make_shared<AsioServer>(this->port(), bus);
  server->start();

  this->connect();

  const auto actual = bus->waitForEvent();
  EXPECT_EQ(EventType::CLIENT_CONNECTED, actual->type());
  // The first client identifier should be 0 as the counter starts from 0
  EXPECT_EQ(ClientId{0}, actual->as<ClientConnectedEvent>().clientId());

  server->stop();
}

TEST_F(Integration_Net_Server_AsioServer, AcceptsMultipleConnections)
{
  // TODO: Implementation
}

TEST_F(Integration_Net_Server_AsioServer, DetectsDisconnectionAndPublishesClientDisconnectedEvent)
{
  auto bus    = std::make_shared<TestEventBus>();
  auto server = std::make_shared<AsioServer>(this->port(), bus);
  server->start();

  auto socket = this->connect();
  auto event  = bus->waitForEvent();
  EXPECT_EQ(EventType::CLIENT_CONNECTED, event->type());
  const auto expectedClientId = event->as<ClientConnectedEvent>().clientId();

  socket->shutdown(asio::ip::tcp::socket::shutdown_both);
  event = bus->waitForEvent();
  EXPECT_EQ(EventType::CLIENT_DISCONNECTED, event->type());
  EXPECT_EQ(expectedClientId, event->as<ClientDisconnectedEvent>().clientId());

  server->stop();
}

TEST_F(Integration_Net_Server_AsioServer, PublishesDataReceivedEventWhenDataIsReceived)
{
  auto bus    = std::make_shared<TestEventBus>();
  auto server = std::make_shared<AsioServer>(this->port(), bus);
  server->start();

  auto socket = this->connect();
  auto event  = bus->waitForEvent();
  EXPECT_EQ(EventType::CLIENT_CONNECTED, event->type());
  const auto expectedClientId = event->as<ClientConnectedEvent>().clientId();

  std::string data("test");
  this->write(socket, data);

  event = bus->waitForEvent();
  EXPECT_EQ(EventType::DATA_RECEIVED, event->type());
  EXPECT_EQ(expectedClientId, event->as<DataReceivedEvent>().clientId());
  const std::vector<char> expectedData(data.begin(), data.end());
  EXPECT_EQ(expectedData, event->as<DataReceivedEvent>().data());

  server->stop();
}

TEST_F(Integration_Net_Server_AsioServer, TerminatesConnectionWhenShuttingDown)
{
  // TODO: Implementation
}

} // namespace net::details
