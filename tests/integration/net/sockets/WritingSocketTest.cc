
#include "WritingSocket.hh"
#include "DataSentEvent.hh"
#include "DataWriteFailureEvent.hh"
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
  auto sockets = this->getTestSockets();
  EXPECT_THROW([&sockets]() { WritingSocket(ClientId{1}, sockets.client, nullptr); }(),
               std::invalid_argument);
}

TEST_F(Integration_Net_Sockets_WritingSocket, PublishesDataSentEvent)
{
  auto sockets = this->getTestSockets();
  auto bus     = std::make_shared<TestEventBus>();
  auto socket  = std::make_shared<WritingSocket>(ClientId{1}, sockets.client, bus);

  std::string data{"test"};
  std::vector<char> toSend(data.begin(), data.end());
  socket->send(MessageId{4}, toSend);

  const auto actual = bus->waitForEvent();
  EXPECT_EQ(EventType::DATA_SENT, actual->type());
  EXPECT_EQ(ClientId{1}, actual->as<DataSentEvent>().clientId());
  EXPECT_EQ(MessageId{4}, actual->as<DataSentEvent>().messageId());
}

TEST_F(Integration_Net_Sockets_WritingSocket, SendsDataToSocket)
{
  auto sockets = this->getTestSockets();
  auto bus     = std::make_shared<TestEventBus>();
  auto socket  = std::make_shared<WritingSocket>(ClientId{1}, sockets.client, std::move(bus));

  std::string data{"test"};
  std::vector<char> toSend(data.begin(), data.end());
  socket->send(MessageId{4}, toSend);

  const auto actual = sockets.readServer(data.size());
  EXPECT_EQ("test", actual);
}

TEST_F(Integration_Net_Sockets_WritingSocket, PublishesDataWriteFailureEventWhenClientSocketIsClosed)
{
  auto sockets = this->getTestSockets();
  auto bus     = std::make_shared<TestEventBus>();
  auto socket  = std::make_shared<WritingSocket>(ClientId{1}, sockets.client, bus);

  sockets.client->shutdown(asio::ip::tcp::socket::shutdown_both);

  std::string data{"test"};
  std::vector<char> toSend(data.begin(), data.end());
  socket->send(MessageId{3}, toSend);

  const auto actual = bus->waitForEvent();
  EXPECT_EQ(EventType::DATA_WRITE_FAILURE, actual->type());
  EXPECT_EQ(ClientId{1}, actual->as<DataWriteFailureEvent>().clientId());
  EXPECT_EQ(MessageId{3}, actual->as<DataWriteFailureEvent>().messageId());
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
  auto sockets = this->getTestSockets();
  auto bus     = std::make_shared<TestEventBus>();
  auto socket  = std::make_shared<WritingSocket>(ClientId{1}, sockets.client, bus);

  sockets.server->shutdown(asio::ip::tcp::socket::shutdown_both);

  std::string data{"test"};
  std::vector<char> toSend(data.begin(), data.end());
  socket->send(MessageId{2}, toSend);

  const auto actual = bus->waitForEvent();
  EXPECT_EQ(EventType::DATA_SENT, actual->type());
  EXPECT_EQ(ClientId{1}, actual->as<DataSentEvent>().clientId());
  EXPECT_EQ(MessageId{2}, actual->as<DataSentEvent>().messageId());
}

} // namespace net::details
