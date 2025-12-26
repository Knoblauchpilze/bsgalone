
#include "WritingSocket.hh"
#include "AsioNetFixture.hh"
#include "DataReader.hh"
#include <gtest/gtest.h>

using namespace ::testing;
using namespace test;

namespace net::details {
using Unit_Net_Asio_WritingSocket = AsioNetFixture;

namespace {
void sendData(WritingSocket &socket, const std::string &data)
{
  std::vector<char> rawData{data.begin(), data.end()};
  socket.send(rawData);
}
} // namespace

TEST_F(Unit_Net_Asio_WritingSocket, SendsDataToSocket)
{
  auto socket = WritingSocket::fromSocket(this->connect());
  auto reader = DataReader::create(this->socket(0));

  std::string data("test");
  sendData(*socket, data);
  this->waitForABit();

  const auto expected = reader->read();

  EXPECT_FALSE(expected.empty());
}

TEST_F(Unit_Net_Asio_WritingSocket, ThrowsErrorOnSecondWriteWhenClientSocketIsClosed)
{
  auto asioSocket = this->connect();
  asioSocket->close();
  auto socket = WritingSocket::fromSocket(std::move(asioSocket));

  std::string data("test");
  sendData(*socket, data);
  this->waitForABit();

  // The first send will fail due to the socket being closed. The second
  // send should fail immediately once the problem has been detected.
  EXPECT_THROW([&socket] { socket->send({}); }(), core::CoreException);
}

} // namespace net::details
