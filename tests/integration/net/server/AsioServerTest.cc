
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
  EXPECT_THROW([this]() { AsioServer(this->asioContext(), this->port(), nullptr); }(),
               std::invalid_argument);
}

TEST_F(Integration_Net_Server_AsioServer, AcceptsConnectionAndPublishesClientConnectedEvent)
{
  auto bus    = std::make_shared<TestEventBus>();
  auto server = std::make_shared<AsioServer>(this->asioContext(), this->port(), bus);
  server->start();

  // The client socket needs to be kept so that the server socket does not
  // detect that it's gone and closes the connection
  auto socket = this->connectToRunningServer();

  const auto actual = bus->waitForEvent();
  EXPECT_EQ(EventType::CLIENT_CONNECTED, actual->type());
  // The first client identifier should be 0 as the counter starts from 0
  EXPECT_EQ(ClientId{0}, actual->as<ClientConnectedEvent>().clientId());
  std::cout << "[test] reached end of test\n";
}

TEST_F(Integration_Net_Server_AsioServer, AcceptsMultipleConnections)
{
  auto bus    = std::make_shared<TestEventBus>();
  auto server = std::make_shared<AsioServer>(this->asioContext(), this->port(), bus);
  std::cout << "[test] starting server\n";
  server->start();

  std::cout << "[test] connecting first socket\n";
  auto socket1 = this->connectToRunningServer();

  std::cout << "[test] waiting for event\n";
  auto actual = bus->waitForEvent();
  EXPECT_EQ(EventType::CLIENT_CONNECTED, actual->type());
  EXPECT_EQ(ClientId{0}, actual->as<ClientConnectedEvent>().clientId());

  std::cout << "[test] connecting second socket\n";
  auto socket2 = this->connectToRunningServer();

  std::cout << "[test] waiting for second event\n";
  actual = bus->waitForEvent();
  EXPECT_EQ(EventType::CLIENT_CONNECTED, actual->type());
  EXPECT_EQ(ClientId{1}, actual->as<ClientConnectedEvent>().clientId());

  std::cout << "[test] end of test\n";
}

TEST_F(Integration_Net_Server_AsioServer, DetectsDisconnectionAndPublishesClientDisconnectedEvent)
{
  auto bus    = std::make_shared<TestEventBus>();
  auto server = std::make_shared<AsioServer>(this->asioContext(), this->port(), bus);
  server->start();

  auto socket = this->connectToRunningServer();
  auto event  = bus->waitForEvent();
  EXPECT_EQ(EventType::CLIENT_CONNECTED, event->type());
  const auto expectedClientId = event->as<ClientConnectedEvent>().clientId();

  socket->shutdown(asio::ip::tcp::socket::shutdown_both);
  event = bus->waitForEvent();
  EXPECT_EQ(EventType::CLIENT_DISCONNECTED, event->type());
  EXPECT_EQ(expectedClientId, event->as<ClientDisconnectedEvent>().clientId());
}

TEST_F(Integration_Net_Server_AsioServer, PublishesDataReceivedEventWhenDataIsReceived)
{
  auto bus    = std::make_shared<TestEventBus>();
  auto server = std::make_shared<AsioServer>(this->asioContext(), this->port(), bus);
  server->start();

  auto socket = this->connectToRunningServer();
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
}

} // namespace net::details
