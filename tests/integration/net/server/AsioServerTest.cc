
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

  std::cout << "[test] started server\n";

  // Async connect to the server to be sure to not miss the event.
  // TODO: It would be better to have a wait or something similar using a
  // condition variable.
  auto cleanup = std::async(std::launch::async, [this]() { this->connect(); });

  std::cout << "[test] got connection\n";

  const auto actual = bus->waitForEvent();
  EXPECT_EQ(EventType::CLIENT_CONNECTED, actual->type());
  EXPECT_EQ(ClientId{0}, actual->as<ClientConnectedEvent>().clientId());

  std::cout << "[test] got event\n";

  cleanup.get();
  std::cout << "[test] async finished\n";
  server->stop();

  std::cout << "[test] stopped server\n";
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

  // First connect to the server. The async is necessary to be sure that we
  // don't miss the event.
  // TODO: It would be better to have a wait or something similar using a
  // condition variable.
  SocketShPtr socket;
  auto cleanup = std::async(std::launch::async, [&socket, this]() { socket = this->connect(); });
  auto event   = bus->waitForEvent();
  EXPECT_EQ(EventType::CLIENT_CONNECTED, event->type());
  const auto clientId = event->as<ClientConnectedEvent>().clientId();
  cleanup.get();

  cleanup = std::async(std::launch::async,
                       [&socket]() { socket->shutdown(asio::ip::tcp::socket::shutdown_both); });
  event   = bus->waitForEvent();
  EXPECT_EQ(EventType::CLIENT_DISCONNECTED, event->type());
  EXPECT_EQ(clientId, event->as<ClientDisconnectedEvent>().clientId());
  cleanup.get();

  server->stop();
}

TEST_F(Integration_Net_Server_AsioServer, PublishesDataReceivedEventWhenDataIsReceived)
{
  auto bus    = std::make_shared<TestEventBus>();
  auto server = std::make_shared<AsioServer>(this->port(), bus);
  server->start();

  // First connect to the server. The async is necessary to be sure that we
  // don't miss the event.
  // TODO: It would be better to have a wait or something similar using a
  // condition variable.
  SocketShPtr socket;
  auto cleanup = std::async(std::launch::async, [&socket, this]() { socket = this->connect(); });
  auto event   = bus->waitForEvent();
  EXPECT_EQ(EventType::CLIENT_CONNECTED, event->type());
  const auto clientId = event->as<ClientConnectedEvent>().clientId();
  cleanup.get();

  std::string data("test");
  this->write(socket, data);
  event = bus->waitForEvent();
  EXPECT_EQ(EventType::DATA_RECEIVED, event->type());
  EXPECT_EQ(clientId, event->as<DataReceivedEvent>().clientId());
  const std::vector<char> expectedData(data.begin(), data.end());
  EXPECT_EQ(expectedData, event->as<DataReceivedEvent>().data());

  server->stop();
}

TEST_F(Integration_Net_Server_AsioServer, TerminatesConnectionWhenShuttingDown)
{
  // TODO: Implementation
}

} // namespace net::details
