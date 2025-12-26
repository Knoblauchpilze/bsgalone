
#include "ReadingSocket.hh"
#include "AsioNetFixture.hh"
#include <gtest/gtest.h>

using namespace ::testing;
using namespace test;

namespace net::details {
using Unit_Net_Asio_ReadingSocket = AsioNetFixture;

namespace {
auto createReadingSocket(net::SocketShPtr asioSocket) -> ReadingSocketShPtr
{
  auto socket = ReadingSocket::fromSocket(std::move(asioSocket));
  socket->connect();
  return socket;
}
} // namespace

TEST_F(Unit_Net_Asio_ReadingSocket, ReturnsNothingWhenNoDataReceived)
{
  auto socket = createReadingSocket(this->connect());

  const auto actual = socket->read();

  EXPECT_TRUE(actual.empty());
}

TEST_F(Unit_Net_Asio_ReadingSocket, ReturnsReceivedData)
{
  auto socket = createReadingSocket(this->connect());

  std::string data("test");
  this->writeTo(0, data);

  const auto actual = socket->read();

  std::vector<char> expected(data.begin(), data.end());
  EXPECT_EQ(expected, actual);
}

TEST_F(Unit_Net_Asio_ReadingSocket, ThrowsErrorWhenClientSocketIsClosed)
{
  auto asioSocket = this->connect();
  asioSocket->close();
  auto socket = std::make_shared<ReadingSocket>(std::move(asioSocket));
  socket->connect();

  this->waitForABit();

  EXPECT_THROW([&socket] { socket->read(); }(), core::CoreException);
}

TEST_F(Unit_Net_Asio_ReadingSocket, FailsToReconnectWhenClientSocketIsClosed)
{
  auto asioSocket = this->connect();
  asioSocket->close();
  auto socket = std::make_shared<ReadingSocket>(std::move(asioSocket));
  socket->connect();
  this->waitForABit();

  EXPECT_THROW([&socket] { socket->connect(); }(), core::CoreException);
}

TEST_F(Unit_Net_Asio_ReadingSocket, ThrowsErrorWhenServerSocketIsClosed)
{
  auto socket = createReadingSocket(this->connect());
  this->socket(0)->close();

  this->waitForABit();

  EXPECT_THROW([&socket] { socket->read(); }(), core::CoreException);
}

} // namespace net::details
