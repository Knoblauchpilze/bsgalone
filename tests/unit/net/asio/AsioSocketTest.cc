
#include "AsioSocket.hh"
#include "AsioNetFixture.hh"
#include "DataReader.hh"
#include <gtest/gtest.h>

using namespace ::testing;
using namespace test;

namespace net::details {
using Unit_Net_Asio_AsioSocket = AsioNetFixture;

namespace {
auto createAsioSocket(net::SocketShPtr asioSocket) -> AsioSocketPtr
{
  auto socket = std::make_unique<AsioSocket>(std::move(asioSocket));
  socket->connect();
  return socket;
}

void sendData(AsioSocket &socket, const std::string &data)
{
  std::vector<char> rawData{data.begin(), data.end()};
  socket.send(rawData);
}
} // namespace

TEST_F(Unit_Net_Asio_AsioSocket, ThrowsExceptionWhenSocketIsNull)
{
  EXPECT_THROW([]() { std::make_unique<AsioSocket>(nullptr); }(), std::invalid_argument);
}

TEST_F(Unit_Net_Asio_AsioSocket, ReturnsNothingWhenNoDataReceived)
{
  auto socket = createAsioSocket(this->connect());

  const auto actual = socket->read();

  EXPECT_TRUE(actual.empty());
}

TEST_F(Unit_Net_Asio_AsioSocket, ReturnsDataWhenReceivedSomething)
{
  auto socket = createAsioSocket(this->connect());

  std::string data("test");
  this->writeTo(0, data);

  const auto actual = socket->read();

  std::vector<char> expected(data.begin(), data.end());
  EXPECT_EQ(expected, actual);
}

TEST_F(Unit_Net_Asio_AsioSocket, SendsDataToSocket)
{
  auto socket = createAsioSocket(this->connect());
  auto reader = DataReader::create(this->socket(0));

  std::string data("test");
  sendData(*socket, data);
  this->waitForABit();

  const auto actual = reader->read();

  std::vector<char> expected(data.begin(), data.end());
  EXPECT_EQ(expected, actual);
}

TEST_F(Unit_Net_Asio_AsioSocket, ReturnsConnectedWhenSocketIsHealthy)
{
  auto socket = createAsioSocket(this->connect());
  EXPECT_TRUE(socket->isConnected());
}

TEST_F(Unit_Net_Asio_AsioSocket, ReturnsDisconnectedWhenClientSocketIsClosed)
{
  auto rawSocket = this->connect();
  rawSocket->close();
  auto socket = createAsioSocket(std::move(rawSocket));
  this->waitForABit();

  EXPECT_FALSE(socket->isConnected());
}

TEST_F(Unit_Net_Asio_AsioSocket, ReturnsDisconnectedWhenServerSocketIsClosed)
{
  std::cout << "[test] creating and connecting socket\n";
  auto socket = createAsioSocket(this->connect());
  std::cout << "[test] closing server socket\n";
  this->socket(0)->close();
  std::cout << "[test] waiting for a bit\n";
  this->waitForABit();

  std::cout << "[test] check\n";
  EXPECT_FALSE(socket->isConnected());
}

TEST_F(Unit_Net_Asio_AsioSocket, ReturnsEndpointAsString)
{
  auto socket = createAsioSocket(this->connect());

  const auto expected = std::format("ON-127.0.0.1:{}", std::to_string(this->port()));
  EXPECT_EQ(expected, socket->endpoint());

  socket->close();

  EXPECT_EQ("OFF-N/A", socket->endpoint());
}

TEST_F(Unit_Net_Asio_AsioSocket, ReturnsEndpointAlsoWhenClientSocketIsClosed)
{
  auto rawSocket = this->connect();
  rawSocket->close();
  auto socket = createAsioSocket(std::move(rawSocket));
  this->waitForABit();

  EXPECT_EQ("OFF-N/A", socket->endpoint());
}

TEST_F(Unit_Net_Asio_AsioSocket, ReturnsEndpointAlsoWhenServerSocketIsClosed)
{
  auto socket = createAsioSocket(this->connect());
  this->socket(0)->close();
  this->waitForABit();

  const auto expected = std::format("OFF-127.0.0.1:{}", std::to_string(this->port()));
  EXPECT_EQ(expected, socket->endpoint());
}
} // namespace net::details
