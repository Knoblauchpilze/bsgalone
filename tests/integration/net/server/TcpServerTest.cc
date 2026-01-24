
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

namespace net {
using Integration_Net_Server_TcpServer = TcpFixture;

namespace {
void startServer(std::shared_ptr<TcpServer> &server,
                 const int port,
                 std::shared_ptr<TestEventBus> &bus)
{
  server->start(port);

  const auto actual = bus->waitForEvent();
  EXPECT_EQ(EventType::SERVER_STARTED, actual->type());
}
} // namespace

TEST_F(Integration_Net_Server_TcpServer, ThrowsWhenEventBusIsNull)
{
  EXPECT_THROW([this]() { TcpServer(nullptr); }(), std::invalid_argument);
}

TEST_F(Integration_Net_Server_TcpServer, PublishesServerStartedEvent)
{
  auto bus    = std::make_shared<TestEventBus>();
  auto server = std::make_shared<TcpServer>(bus);
  server->start(this->port());

  const auto actual = bus->waitForEvent();
  EXPECT_EQ(EventType::SERVER_STARTED, actual->type());
}

TEST_F(Integration_Net_Server_TcpServer, PublishesServerStoppedEvent)
{
  auto bus    = std::make_shared<TestEventBus>();
  auto server = std::make_shared<TcpServer>(bus);
  startServer(server, this->port(), bus);

  server->stop();

  const auto actual = bus->waitForEvent();
  EXPECT_EQ(EventType::SERVER_STOPPED, actual->type());
}

TEST_F(Integration_Net_Server_TcpServer, AcceptsConnectionAndPublishesClientConnectedEvent)
{
  auto bus    = std::make_shared<TestEventBus>();
  auto server = std::make_shared<TcpServer>(bus);
  startServer(server, this->port(), bus);

  // The client socket needs to be kept so that the server socket does not
  // detect that it's gone and closes the connection
  auto socket = this->connectToRunningServer();

  const auto actual = bus->waitForEvent();
  EXPECT_EQ(EventType::CLIENT_CONNECTED, actual->type());
  // The first client identifier should be 0 as the counter starts from 0
  EXPECT_EQ(ClientId{0}, actual->as<ClientConnectedEvent>().clientId());

  server->stop();
}

TEST_F(Integration_Net_Server_TcpServer, DetectsDisconnectionAndPublishesClientDisconnectedEvent)
{
  auto bus    = std::make_shared<TestEventBus>();
  auto server = std::make_shared<TcpServer>(bus);
  startServer(server, this->port(), bus);

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
  startServer(server, this->port(), bus);

  auto socket = this->connectToRunningServer();
  auto event  = bus->waitForEvent();
  EXPECT_EQ(EventType::CLIENT_CONNECTED, event->type());
  const auto expectedClientId = event->as<ClientConnectedEvent>().clientId();

  server->stop();

  event = bus->waitForEvent(EventType::CLIENT_DISCONNECTED);
  EXPECT_EQ(EventType::CLIENT_DISCONNECTED, event->type());
  EXPECT_EQ(expectedClientId, event->as<ClientDisconnectedEvent>().clientId());
}

} // namespace net
