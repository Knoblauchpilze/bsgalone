
#include "ReadingSocket.hh"
#include "AsioServerFixture.hh"
#include <gtest/gtest.h>

using namespace ::testing;
using namespace test;

namespace net::details {
using Unit_Net_Sockets_ReadingSocket = AsioServerFixture;

namespace {
auto createReadingSocket(net::SocketShPtr asioSocket) -> ReadingSocketShPtr
{
  auto socket = ReadingSocket::fromSocket(std::move(asioSocket));
  socket->connect();
  return socket;
}
} // namespace

TEST_F(Unit_Net_Sockets_ReadingSocket, ReturnsNothingWhenNoDataReceived)
{
  auto socket = createReadingSocket(this->connect());

  const auto actual = socket->read();

  EXPECT_TRUE(actual.empty());
}

TEST_F(Unit_Net_Sockets_ReadingSocket, ReturnsReceivedData)
{
  auto socket = createReadingSocket(this->connect());

  std::string data("test");
  this->writeTo(0, data);

  const auto actual = socket->read();

  std::vector<char> expected(data.begin(), data.end());
  EXPECT_EQ(expected, actual);
}

TEST_F(Unit_Net_Sockets_ReadingSocket, ThrowsErrorWhenClientSocketIsClosed)
{
  auto asioSocket = this->connect();
  asioSocket->close();
  auto socket = std::make_shared<ReadingSocket>(std::move(asioSocket));
  socket->connect();

  this->waitForABit();

  EXPECT_THROW([&socket] { socket->read(); }(), core::CoreException);
}

TEST_F(Unit_Net_Sockets_ReadingSocket, FailsToReconnectWhenClientSocketIsClosed)
{
  auto asioSocket = this->connect();
  asioSocket->close();
  auto socket = std::make_shared<ReadingSocket>(std::move(asioSocket));
  socket->connect();
  this->waitForABit();

  EXPECT_THROW([&socket] { socket->connect(); }(), core::CoreException);
}

TEST_F(Unit_Net_Sockets_ReadingSocket, ThrowsErrorWhenServerSocketIsClosed)
{
  auto socket = createReadingSocket(this->connect());

  this->socket(0)->close();
  this->waitForABit();

  EXPECT_THROW([&socket] { socket->read(); }(), core::CoreException);
}

TEST_F(Unit_Net_Sockets_ReadingSocket, ReturnsConnectedWhenSocketIsHealthy)
{
  auto socket = createReadingSocket(this->connect());

  EXPECT_TRUE(socket->isConnected());
}

TEST_F(Unit_Net_Sockets_ReadingSocket, ReturnsDisconnectedWhenClientSocketIsClosed)
{
  auto asioSocket = this->connect();
  asioSocket->close();
  auto socket = createReadingSocket(std::move(asioSocket));

  this->waitForABit();

  EXPECT_FALSE(socket->isConnected());
}

TEST_F(Unit_Net_Sockets_ReadingSocket, ReturnsDisconnectedWhenServerSocketIsClosed)
{
  auto socket = createReadingSocket(this->connect());

  this->socket(0)->close();
  this->waitForABit();

  EXPECT_FALSE(socket->isConnected());
}

} // namespace net::details
