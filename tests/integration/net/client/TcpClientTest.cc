
#include "TcpClient.hh"
#include "ConnectedSockets.hh"
#include "DataReceivedEvent.hh"
#include "DataSentEvent.hh"
#include "TcpServerFixture.hh"
#include "TestEventBus.hh"
#include <gtest/gtest.h>

using namespace test;
using namespace ::testing;

namespace net {
using Integration_Net_Client_TcpClient = TcpServerFixture;

constexpr auto LOCALHOST_URL = "127.0.0.1";

namespace {
void startClient(std::shared_ptr<TcpClient> &client, const int port, TestEventBusShPtr &bus)
{
  client->connect(LOCALHOST_URL, port);

  const auto actual = bus->waitForEvent();
  EXPECT_EQ(EventType::SERVER_STARTED, actual->type());
}
} // namespace

TEST_F(Integration_Net_Client_TcpClient, ThrowsWhenEventBusIsNull)
{
  EXPECT_THROW([this]() { TcpClient(nullptr); }(), std::invalid_argument);
}

TEST_F(Integration_Net_Client_TcpClient, ThrowsWhenConnectingMultipleTimes)
{
  auto bus    = std::make_shared<TestEventBus>();
  auto client = std::make_shared<TcpClient>(bus);
  client->connect(LOCALHOST_URL, this->port());

  auto body = [this, &client]() { client->connect(LOCALHOST_URL, this->port()); };

  EXPECT_THROW(body(), core::CoreException);
}

TEST_F(Integration_Net_Client_TcpClient, ThrowsWhenDisconnectingWithoutConnecting)
{
  auto bus    = std::make_shared<TestEventBus>();
  auto client = std::make_shared<TcpClient>(bus);

  EXPECT_THROW([&client]() { client->disconnect(); }(), core::CoreException);
}

TEST_F(Integration_Net_Client_TcpClient, PublishesServerStartedEvent)
{
  auto bus    = std::make_shared<TestEventBus>();
  auto client = std::make_shared<TcpClient>(bus);
  client->connect(LOCALHOST_URL, this->port());

  const auto actual = bus->waitForEvent();
  EXPECT_EQ(EventType::SERVER_STARTED, actual->type());
}

TEST_F(Integration_Net_Client_TcpClient, PublishesServerStoppedEvent)
{
  auto bus    = std::make_shared<TestEventBus>();
  auto client = std::make_shared<TcpClient>(bus);
  startClient(client, this->port(), bus);
  this->waitForConnectionEstablishedEvent(bus);

  client->disconnect();

  // The event bus will receive both a client disconnected event and the
  // server stopped event. This test only cares about the server stopped
  // one.
  bus->waitForEvent(EventType::SERVER_STOPPED);
}

TEST_F(Integration_Net_Client_TcpClient, EstablishesConnectionAndPublishesConnectionEstablishedEvent)
{
  auto bus    = std::make_shared<TestEventBus>();
  auto client = std::make_shared<TcpClient>(bus);
  startClient(client, this->port(), bus);

  const auto sockets = this->waitForServerSocket();

  const auto actual = bus->waitForEvent();
  EXPECT_EQ(EventType::CONNECTION_ESTABLISHED, actual->type());
}

TEST_F(Integration_Net_Client_TcpClient, DetectsDisconnectionAndPublishesConnectionLostEvent)
{
  auto bus    = std::make_shared<TestEventBus>();
  auto client = std::make_shared<TcpClient>(bus);
  startClient(client, this->port(), bus);

  const auto sockets = this->waitForConnectionEstablishedEvent(bus);

  sockets.server->shutdown(asio::ip::tcp::socket::shutdown_both);
  const auto event = bus->waitForEvent();

  EXPECT_EQ(EventType::CONNECTION_LOST, event->type());
}

TEST_F(Integration_Net_Client_TcpClient, PublishesConnectionLostEventWhenClientDisconnects)
{
  auto bus    = std::make_shared<TestEventBus>();
  auto client = std::make_shared<TcpClient>(bus);
  startClient(client, this->port(), bus);

  this->waitForConnectionEstablishedEvent(bus);
  client->disconnect();

  bus->waitForEvent(EventType::CONNECTION_LOST);
}

TEST_F(Integration_Net_Client_TcpClient, SendsDataToServerSocket)
{
  auto bus    = std::make_shared<TestEventBus>();
  auto client = std::make_shared<TcpClient>(bus);
  startClient(client, this->port(), bus);

  const auto sockets = this->waitForConnectionEstablishedEvent(bus);

  std::string data("test");
  client->trySend(std::vector<char>(data.begin(), data.end()));

  const auto actual = sockets.readServer(data.size());
  EXPECT_EQ("test", actual);
}

TEST_F(Integration_Net_Client_TcpClient, PublishesDataSentEvent)
{
  auto bus    = std::make_shared<TestEventBus>();
  auto client = std::make_shared<TcpClient>(bus);
  startClient(client, this->port(), bus);

  const auto sockets = this->waitForConnectionEstablishedEvent(bus);

  std::string data("test");
  const auto expectedMessageId = client->trySend(std::vector<char>(data.begin(), data.end()));
  EXPECT_TRUE(expectedMessageId.has_value());

  const auto event = bus->waitForEvent();
  EXPECT_EQ(EventType::DATA_SENT, event->type());
  EXPECT_FALSE(event->as<DataSentEvent>().tryGetClientId().has_value());
  const std::vector<char> expectedData(data.begin(), data.end());
  EXPECT_EQ(expectedMessageId.value(), event->as<DataSentEvent>().messageId());

  client->disconnect();
}

} // namespace net
