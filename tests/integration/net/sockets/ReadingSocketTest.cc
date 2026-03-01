
#include "ReadingSocket.hh"
#include "DataReadFailureEvent.hh"
#include "DataReceivedEvent.hh"
#include "TcpServerFixture.hh"
#include "TestNetworkEventQueue.hh"
#include <gmock/gmock-matchers.h>
#include <gtest/gtest.h>

using namespace test;
using namespace ::testing;

namespace net::details {
using Integration_Net_Sockets_ReadingSocket = TcpServerFixture;

TEST_F(Integration_Net_Sockets_ReadingSocket, ThrowsWhenSocketIsNull)
{
  EXPECT_THROW(
    [] { ReadingSocket(ClientId{1}, nullptr, std::make_shared<TestNetworkEventQueue>()); }(),
    std::invalid_argument);
}

TEST_F(Integration_Net_Sockets_ReadingSocket, ThrowsWhenEventBusIsNull)
{
  auto sockets = this->getTestSockets();
  EXPECT_THROW([&sockets]() { ReadingSocket(ClientId{1}, sockets.client, nullptr); }(),
               std::invalid_argument);
}

TEST_F(Integration_Net_Sockets_ReadingSocket, ThrowsWhenConnectingTwice)
{
  auto sockets = this->getTestSockets();
  auto bus     = std::make_shared<TestNetworkEventQueue>();
  auto socket  = std::make_shared<ReadingSocket>(ClientId{1}, sockets.client, std::move(bus));
  socket->connect();

  EXPECT_THROW([&socket]() { socket->connect(); }(), core::CoreException);
}

TEST_F(Integration_Net_Sockets_ReadingSocket, PublishesDataReceivedEvent)
{
  auto sockets = this->getTestSockets();
  auto bus     = std::make_shared<TestNetworkEventQueue>();
  auto socket  = std::make_shared<ReadingSocket>(ClientId{1}, sockets.client, bus);
  socket->connect();

  std::string data{"test"};
  sockets.writeServer(data);

  const auto actual = bus->waitForEvent();
  EXPECT_EQ(EventType::DATA_RECEIVED, actual->type());
  EXPECT_EQ(ClientId{1}, actual->as<DataReceivedEvent>().tryGetClientId().value());
  const std::vector<char> expectedData(data.begin(), data.end());
  EXPECT_EQ(expectedData, actual->as<DataReceivedEvent>().data());
}

TEST_F(Integration_Net_Sockets_ReadingSocket,
       PublishesDataReceivedEventWithNoClientIdWhenNotSpecified)
{
  auto sockets = this->getTestSockets();
  auto bus     = std::make_shared<TestNetworkEventQueue>();
  auto socket  = std::make_shared<ReadingSocket>(sockets.client, bus);
  socket->connect();

  std::string data{"test"};
  sockets.writeServer(data);

  const auto actual = bus->waitForEvent();
  EXPECT_EQ(EventType::DATA_RECEIVED, actual->type());
  EXPECT_FALSE(actual->as<DataReceivedEvent>().tryGetClientId().has_value());
  const std::vector<char> expectedData(data.begin(), data.end());
  EXPECT_EQ(expectedData, actual->as<DataReceivedEvent>().data());
}

TEST_F(Integration_Net_Sockets_ReadingSocket, PublishesDataReadFailureEventWhenServerSocketIsClosed)
{
  auto sockets = this->getTestSockets();
  auto bus     = std::make_shared<TestNetworkEventQueue>();
  auto socket  = std::make_shared<ReadingSocket>(ClientId{1}, sockets.client, bus);
  socket->connect();

  sockets.server->shutdown(asio::ip::tcp::socket::shutdown_both);

  const auto actual = bus->waitForEvent();
  EXPECT_EQ(EventType::DATA_READ_FAILURE, actual->type());
  EXPECT_EQ(ClientId{1}, actual->as<DataReadFailureEvent>().tryGetClientId().value());
}

TEST_F(Integration_Net_Sockets_ReadingSocket,
       PublishesDataReadFailureEventWithNoClientIdentifierWhenServerSocketIsClosed)
{
  auto sockets = this->getTestSockets();
  auto bus     = std::make_shared<TestNetworkEventQueue>();
  auto socket  = std::make_shared<ReadingSocket>(sockets.client, bus);
  socket->connect();

  sockets.server->shutdown(asio::ip::tcp::socket::shutdown_both);

  const auto actual = bus->waitForEvent();
  EXPECT_EQ(EventType::DATA_READ_FAILURE, actual->type());
  EXPECT_FALSE(actual->as<DataReadFailureEvent>().tryGetClientId().has_value());
}

TEST_F(Integration_Net_Sockets_ReadingSocket, PublishesDataReadFailureEventWhenClientSocketIsClosed)
{
  auto sockets = this->getTestSockets();
  auto bus     = std::make_shared<TestNetworkEventQueue>();
  auto socket  = std::make_shared<ReadingSocket>(ClientId{1}, sockets.client, bus);
  socket->connect();

  sockets.client->shutdown(asio::ip::tcp::socket::shutdown_both);

  const auto actual = bus->waitForEvent();
  EXPECT_EQ(EventType::DATA_READ_FAILURE, actual->type());
  EXPECT_EQ(ClientId{1}, actual->as<DataReadFailureEvent>().tryGetClientId().value());
}

TEST_F(Integration_Net_Sockets_ReadingSocket, FailsToReconnectWhenSocketIsDisconnected)
{
  auto sockets = this->getTestSockets();
  auto bus     = std::make_shared<TestNetworkEventQueue>();
  auto socket  = std::make_shared<ReadingSocket>(ClientId{1}, sockets.client, bus);
  socket->connect();

  sockets.server->shutdown(asio::ip::tcp::socket::shutdown_both);
  bus->waitForEvent(EventType::DATA_READ_FAILURE);

  EXPECT_THAT([&socket]() { socket->connect(); },
              ThrowsMessage<core::CoreException>(HasSubstr("Cannot connect closed socket")));
}

} // namespace net::details
