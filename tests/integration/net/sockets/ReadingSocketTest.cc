
#include "ReadingSocket.hh"
#include "ClientDisconnectedEvent.hh"
#include "DataReceivedEvent.hh"
#include "IEventBus.hh"
#include "TcpServerFixture.hh"
#include "TestEventBus.hh"
#include <deque>
#include <gmock/gmock-matchers.h>
#include <gtest/gtest.h>

using namespace test;
using namespace ::testing;

namespace net::details {
using Integration_Net_Sockets_ReadingSocket = TcpServerFixture;

TEST_F(Integration_Net_Sockets_ReadingSocket, ThrowsWhenSocketIsNull)
{
  EXPECT_THROW(
    [] {
      TestEventBus bus;
      ReadingSocket(ClientId{1}, nullptr, &bus);
    }(),
    std::invalid_argument);
}

TEST_F(Integration_Net_Sockets_ReadingSocket, ThrowsWhenEventBusIsNull)
{
  auto tcpSocket = this->connect();
  EXPECT_THROW([&tcpSocket]() { ReadingSocket(ClientId{1}, tcpSocket, nullptr); }(),
               std::invalid_argument);
}

TEST_F(Integration_Net_Sockets_ReadingSocket, ThrowsWhenConnectingTwice)
{
  auto tcpSocket = this->connect();
  TestEventBus bus;
  auto socket = std::make_shared<ReadingSocket>(ClientId{1}, tcpSocket, &bus);
  socket->connect();

  EXPECT_THROW([&socket]() { socket->connect(); }(), core::CoreException);
}

TEST_F(Integration_Net_Sockets_ReadingSocket, PublishesDataReceivedEvent)
{
  auto tcpSocket = this->connect();
  TestEventBus bus;
  auto socket = std::make_shared<ReadingSocket>(ClientId{1}, tcpSocket, &bus);
  socket->connect();

  std::string data{"test"};
  this->write(0, data);

  auto actual = bus.waitForEvent();
  EXPECT_EQ(EventType::DATA_RECEIVED, actual->type());
  EXPECT_EQ(ClientId{1}, actual->as<DataReceivedEvent>().clientId());
  const std::vector<char> expectedData(data.begin(), data.end());
  EXPECT_EQ(expectedData, actual->as<DataReceivedEvent>().data());
}

TEST_F(Integration_Net_Sockets_ReadingSocket, PublishesClientDisconnectedEventWhenSocketIsClosed)
{
  auto tcpSocket = this->connect();
  TestEventBus bus;
  auto socket = std::make_shared<ReadingSocket>(ClientId{1}, tcpSocket, &bus);
  socket->connect();

  // Disconnect the server socket after some time.
  auto cleanup = std::async(std::launch::async, [this]() {
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    this->serverSocket(0)->close();
  });

  auto actual = bus.waitForEvent();
  EXPECT_EQ(EventType::CLIENT_DISCONNECTED, actual->type());
  EXPECT_EQ(ClientId{1}, actual->as<ClientDisconnectedEvent>().clientId());

  cleanup.get();
}

TEST_F(Integration_Net_Sockets_ReadingSocket, FailsToReconnectWhenSocketIsDisconnected)
{
  auto tcpSocket = this->connect();
  TestEventBus bus;
  auto socket = std::make_shared<ReadingSocket>(ClientId{1}, tcpSocket, &bus);
  socket->connect();

  this->serverSocket(0)->close();

  auto actual = bus.waitForEvent();
  EXPECT_EQ(EventType::CLIENT_DISCONNECTED, actual->type());

  EXPECT_THAT([&socket]() { socket->connect(); },
              ThrowsMessage<core::CoreException>(HasSubstr("Cannot connect closed socket")));
}

} // namespace net::details
