
#include "TcpServer.hh"
#include "ClientConnectedEvent.hh"
#include "ClientDisconnectedEvent.hh"
#include "ConnectedSockets.hh"
#include "DataReceivedEvent.hh"
#include "DataSentEvent.hh"
#include "TcpFixture.hh"
#include "TestEventBus.hh"
#include <gtest/gtest.h>

using namespace test;
using namespace ::testing;

namespace net::details {
using Integration_Net_Server_TcpServer = TcpFixture;

TEST_F(Integration_Net_Server_TcpServer, ThrowsWhenEventBusIsNull)
{
  EXPECT_THROW([this]() { TcpServer(nullptr); }(), std::invalid_argument);
}

TEST_F(Integration_Net_Server_TcpServer, AcceptsConnectionAndPublishesClientConnectedEvent)
{
  auto bus    = std::make_shared<TestEventBus>();
  auto server = std::make_shared<TcpServer>(bus);
  server->start(this->port());

  // The client socket needs to be kept so that the server socket does not
  // detect that it's gone and closes the connection
  auto socket = this->connectToRunningServer();

  const auto actual = bus->waitForEvent();
  EXPECT_EQ(EventType::CLIENT_CONNECTED, actual->type());
  // The first client identifier should be 0 as the counter starts from 0
  EXPECT_EQ(ClientId{0}, actual->as<ClientConnectedEvent>().clientId());
}

TEST_F(Integration_Net_Server_TcpServer, DetectsDisconnectionAndPublishesClientDisconnectedEvent)
{
  auto bus    = std::make_shared<TestEventBus>();
  auto server = std::make_shared<TcpServer>(bus);
  server->start(this->port());

  auto socket = this->connectToRunningServer();
  auto event  = bus->waitForEvent();
  EXPECT_EQ(EventType::CLIENT_CONNECTED, event->type());
  const auto expectedClientId = event->as<ClientConnectedEvent>().clientId();

  socket->shutdown(asio::ip::tcp::socket::shutdown_both);
  event = bus->waitForEvent();
  EXPECT_EQ(EventType::CLIENT_DISCONNECTED, event->type());
  EXPECT_EQ(expectedClientId, event->as<ClientDisconnectedEvent>().clientId());
}

TEST_F(Integration_Net_Server_TcpServer, PublishesClientDisconnectedEventWhenServerIsStopped)
{
  auto bus    = std::make_shared<TestEventBus>();
  auto server = std::make_shared<TcpServer>(bus);
  server->start(this->port());

  auto socket = this->connectToRunningServer();
  auto event  = bus->waitForEvent();
  EXPECT_EQ(EventType::CLIENT_CONNECTED, event->type());
  const auto expectedClientId = event->as<ClientConnectedEvent>().clientId();

  server->stop();

  event = bus->waitForEvent();
  EXPECT_EQ(EventType::CLIENT_DISCONNECTED, event->type());
  EXPECT_EQ(expectedClientId, event->as<ClientDisconnectedEvent>().clientId());
}

} // namespace net::details
