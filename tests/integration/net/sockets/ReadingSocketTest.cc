
#include "ReadingSocket.hh"
#include "ClientDisconnectedEvent.hh"
#include "DataReceivedEvent.hh"
#include "IEventBus.hh"
#include "TcpServerFixture.hh"
#include "TestEventBus.hh"
#include <gmock/gmock-matchers.h>
#include <gtest/gtest.h>

using namespace test;
using namespace ::testing;

namespace net::details {
using Integration_Net_Sockets_ReadingSocket = TcpServerFixture;

TEST_F(Integration_Net_Sockets_ReadingSocket, ThrowsWhenSocketIsNull)
{
  EXPECT_THROW([] { ReadingSocket(ClientId{1}, nullptr, std::make_shared<TestEventBus>()); }(),
               std::invalid_argument);
}

TEST_F(Integration_Net_Sockets_ReadingSocket, ThrowsWhenEventBusIsNull)
{
  auto sockets = this->connectBoth();
  EXPECT_THROW([&sockets]() { ReadingSocket(ClientId{1}, sockets.client, nullptr); }(),
               std::invalid_argument);
}

TEST_F(Integration_Net_Sockets_ReadingSocket, ThrowsWhenConnectingTwice)
{
  auto tcpSocket = this->connect();
  auto bus       = std::make_shared<TestEventBus>();
  auto socket    = std::make_shared<ReadingSocket>(ClientId{1}, tcpSocket, std::move(bus));
  socket->connect();

  EXPECT_THROW([&socket]() { socket->connect(); }(), core::CoreException);
}

TEST_F(Integration_Net_Sockets_ReadingSocket, PublishesDataReceivedEvent)
{
  auto sockets = this->connectBoth();
  auto bus     = std::make_shared<TestEventBus>();
  auto socket  = std::make_shared<ReadingSocket>(ClientId{1}, sockets.client, bus);
  socket->connect();

  std::string data{"test"};
  sockets.writeServer(data);

  const auto actual = bus->waitForEvent();
  EXPECT_EQ(EventType::DATA_RECEIVED, actual->type());
  EXPECT_EQ(ClientId{1}, actual->as<DataReceivedEvent>().clientId());
  const std::vector<char> expectedData(data.begin(), data.end());
  EXPECT_EQ(expectedData, actual->as<DataReceivedEvent>().data());
}

TEST_F(Integration_Net_Sockets_ReadingSocket, PublishesClientDisconnectedEventWhenSocketIsClosed)
{
  auto sockets = this->connectBoth();
  auto bus     = std::make_shared<TestEventBus>();
  auto socket  = std::make_shared<ReadingSocket>(ClientId{1}, sockets.client, bus);
  socket->connect();

  sockets.server->shutdown(asio::ip::tcp::socket::shutdown_both);

  const auto actual = bus->waitForEvent();
  EXPECT_EQ(EventType::CLIENT_DISCONNECTED, actual->type());
  EXPECT_EQ(ClientId{1}, actual->as<ClientDisconnectedEvent>().clientId());
}

TEST_F(Integration_Net_Sockets_ReadingSocket, FailsToReconnectWhenSocketIsDisconnected)
{
  auto sockets = this->connectBoth();
  auto bus     = std::make_shared<TestEventBus>();
  auto socket  = std::make_shared<ReadingSocket>(ClientId{1}, sockets.client, bus);
  socket->connect();

  sockets.server->shutdown(asio::ip::tcp::socket::shutdown_both);

  const auto actual = bus->waitForEvent();
  EXPECT_EQ(EventType::CLIENT_DISCONNECTED, actual->type());

  EXPECT_THAT([&socket]() { socket->connect(); },
              ThrowsMessage<core::CoreException>(HasSubstr("Cannot connect closed socket")));
}

} // namespace net::details
