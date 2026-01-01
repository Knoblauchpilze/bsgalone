
#include "WritingSocket.hh"
#include "ClientDisconnectedEvent.hh"
#include "DataSentEvent.hh"
#include "IEventBus.hh"
#include "TcpServerFixture.hh"
#include "TestEventBus.hh"
#include <gmock/gmock-matchers.h>
#include <gtest/gtest.h>

using namespace test;
using namespace ::testing;

namespace net::details {
using Integration_Net_Sockets_WritingSocket = TcpServerFixture;

TEST_F(Integration_Net_Sockets_WritingSocket, ThrowsWhenSocketIsNull)
{
  EXPECT_THROW([] { WritingSocket(ClientId{1}, nullptr, std::make_shared<TestEventBus>()); }(),
               std::invalid_argument);
}

TEST_F(Integration_Net_Sockets_WritingSocket, ThrowsWhenEventBusIsNull)
{
  auto tcpSocket = this->connect();
  EXPECT_THROW([&tcpSocket]() { WritingSocket(ClientId{1}, tcpSocket, nullptr); }(),
               std::invalid_argument);
}

TEST_F(Integration_Net_Sockets_WritingSocket, PublishesDataSentEvent)
{
  auto tcpSocket = this->connect();
  auto bus       = std::make_shared<TestEventBus>();
  auto socket    = std::make_shared<WritingSocket>(ClientId{1}, tcpSocket, bus);

  std::string data{"test"};
  // Send the data async to be sure to not miss the event.
  auto cleanup = std::async(std::launch::async, [&socket, &data]() {
    std::vector<char> toSend(data.begin(), data.end());
    socket->send(toSend);
  });

  auto actual = bus->waitForEvent();
  EXPECT_EQ(EventType::DATA_SENT, actual->type());
  EXPECT_EQ(ClientId{1}, actual->as<DataSentEvent>().clientId());

  cleanup.get();
}

// TODO: This test crashes with the following logs:
/// [test] bus = 0x7ffc1f4dece0
/// [test] read
/// [writing] async write for test
/// [test] read some data: test
/// [test] waiting
/// [test] this is the end
/// [fixture] tear down
/// [writing] onDataSent: 0, message: Success, length = 4
/// [writing] publish
/// [writing] push, bus: not null: 0x7ffc1f4dece0
/// Segmentation fault (core dumped)
// This is because we clean up the test before the socket is cleaned
// it leaves on because asio has a shared ptr on it, but the test event
// bus does not live on.
// Maybe we should use shared ptr for the event bus?
// It is not clear why the onDataSent can still be invoked even though
// the context has been stopped already?
// It's not said that the existing operation will stop though.
TEST_F(Integration_Net_Sockets_WritingSocket, SendsDataToSocket)
{
  auto tcpSocket = this->connect();
  auto bus       = std::make_shared<TestEventBus>();
  auto socket    = std::make_shared<WritingSocket>(ClientId{1}, tcpSocket, std::move(bus));

  std::string data{"test"};
  // Send the data async to be sure to not miss the event.
  auto cleanup = std::async(std::launch::async, [&socket, &data]() {
    std::vector<char> toSend(data.begin(), data.end());
    socket->send(toSend);
  });

  const auto actual = this->read(0, data.size());
  EXPECT_EQ("test", actual);

  cleanup.get();
}

TEST_F(Integration_Net_Sockets_WritingSocket,
       PublishesClientDisconnectedEventWhenClientSocketIsClosed)
{
  auto tcpSocket = this->connect();
  auto bus       = std::make_shared<TestEventBus>();
  auto socket    = std::make_shared<WritingSocket>(ClientId{1}, tcpSocket, bus);

  // Disconnect the client socket and then send data
  auto cleanup = std::async(std::launch::async, [&tcpSocket, &socket]() {
    tcpSocket->shutdown(asio::ip::tcp::socket::shutdown_both);

    std::string data{"test"};
    std::vector<char> toSend(data.begin(), data.end());
    socket->send(toSend);
  });

  auto actual = bus->waitForEvent();
  EXPECT_EQ(EventType::CLIENT_DISCONNECTED, actual->type());
  EXPECT_EQ(ClientId{1}, actual->as<ClientDisconnectedEvent>().clientId());

  cleanup.get();
}

// Note: this test shows that we can't detect failures when writing. Fortunately
// this is not a big issue for now as there will always be both a reading and a
// writing socket.
// Solving this issue seems to rely on using an IO service. See:
// https://stackoverflow.com/questions/17530912/why-doesnt-boost-asio-async-write-detect-that-socket-has-been-closed
// https://gist.github.com/hoffis/024f60456595e8f1592aa5983f87ead1
// However the service is deprecated: github.com/boostorg/asio/issues/110
TEST_F(Integration_Net_Sockets_WritingSocket, SucceedsToSendDataEvenWhenServerSocketIsClosed)
{
  auto tcpSocket = this->connect();
  auto bus       = std::make_shared<TestEventBus>();
  auto socket    = std::make_shared<WritingSocket>(ClientId{1}, tcpSocket, bus);

  // Disconnect the server socket and then send data
  auto cleanup = std::async(std::launch::async, [this, &socket]() {
    this->serverSocket(0)->shutdown(asio::ip::tcp::socket::shutdown_both);

    std::string data{"test"};
    std::vector<char> toSend(data.begin(), data.end());
    socket->send(toSend);
  });

  auto actual = bus->waitForEvent();
  EXPECT_EQ(EventType::DATA_SENT, actual->type());
  EXPECT_EQ(ClientId{1}, actual->as<DataSentEvent>().clientId());

  cleanup.get();
}

} // namespace net::details
