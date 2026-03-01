
#include "AsioClient.hh"
#include "ConnectionEstablishedEvent.hh"
#include "ConnectionLostEvent.hh"
#include "DataReceivedEvent.hh"
#include "DataSentEvent.hh"
#include "TcpServerFixture.hh"
#include "TestNetworkEventQueue.hh"
#include <gtest/gtest.h>

using namespace test;
using namespace ::testing;

namespace net::details {
using Integration_Net_Client_AsioClient = TcpServerFixture;

constexpr auto LOCALHOST_URL = "127.0.0.1";

TEST_F(Integration_Net_Client_AsioClient, ThrowsWhenEventBusIsNull)
{
  EXPECT_THROW([this]() { AsioClient(nullptr); }(), std::invalid_argument);
}

TEST_F(Integration_Net_Client_AsioClient, ConnectsToServerAndPublishesConnectionEstablishedEvent)
{
  auto bus    = std::make_shared<TestNetworkEventQueue>();
  auto client = std::make_shared<AsioClient>(bus);
  client->connect(this->asioContext(), LOCALHOST_URL, this->port());

  const auto sockets = this->waitForServerSocket();

  const auto actual = bus->waitForEvent();
  EXPECT_EQ(EventType::CONNECTION_ESTABLISHED, actual->type());
}

TEST_F(Integration_Net_Client_AsioClient, DetectsDisconnectionAndPublishesConnectionLostEvent)
{
  auto bus    = std::make_shared<TestNetworkEventQueue>();
  auto client = std::make_shared<AsioClient>(bus);
  client->connect(this->asioContext(), LOCALHOST_URL, this->port());

  const auto sockets = this->waitForConnectionEstablishedEvent(bus);
  sockets.server->shutdown(asio::ip::tcp::socket::shutdown_both);

  const auto event = bus->waitForEvent();
  EXPECT_EQ(EventType::CONNECTION_LOST, event->type());
}

TEST_F(Integration_Net_Client_AsioClient, PublishesDataReceivedEventWhenDataIsReceived)
{
  auto bus    = std::make_shared<TestNetworkEventQueue>();
  auto client = std::make_shared<AsioClient>(bus);
  client->connect(this->asioContext(), LOCALHOST_URL, this->port());

  const auto sockets = this->waitForConnectionEstablishedEvent(bus);

  std::string data("test");
  sockets.writeServer(data);

  const auto event = bus->waitForEvent();
  EXPECT_EQ(EventType::DATA_RECEIVED, event->type());
  EXPECT_FALSE(event->as<DataReceivedEvent>().tryGetClientId().has_value());
  const std::vector<char> expectedData(data.begin(), data.end());
  EXPECT_EQ(expectedData, event->as<DataReceivedEvent>().data());
}

TEST_F(Integration_Net_Client_AsioClient, PublishesConnectionLostEventWhenDisconnectIsCalled)
{
  auto bus = std::make_shared<TestNetworkEventQueue>();

  auto client = std::make_shared<AsioClient>(bus);
  client->connect(this->asioContext(), LOCALHOST_URL, this->port());

  this->waitForConnectionEstablishedEvent(bus);

  client->disconnect();

  const auto event = bus->waitForEvent();
  EXPECT_EQ(EventType::CONNECTION_LOST, event->type());
}

TEST_F(Integration_Net_Client_AsioClient, SucceedsWhenDisconnectingAndNotConnected)
{
  auto bus    = std::make_shared<TestNetworkEventQueue>();
  auto client = std::make_shared<AsioClient>(bus);

  client->disconnect();
}

TEST_F(Integration_Net_Client_AsioClient, ThrowsWhenWritingAMessageAndNotConnected)
{
  auto bus    = std::make_shared<TestNetworkEventQueue>();
  auto client = std::make_shared<AsioClient>(bus);

  EXPECT_THROW([&client]() { client->trySend(std::vector<char>(2, 0)); }(), core::CoreException);
}

TEST_F(Integration_Net_Client_AsioClient, ReturnsEmptyMessageIdentifierWhenMessageIsEmpty)
{
  auto bus    = std::make_shared<TestNetworkEventQueue>();
  auto client = std::make_shared<AsioClient>(bus);
  client->connect(this->asioContext(), LOCALHOST_URL, this->port());

  this->waitForConnectionEstablishedEvent(bus);

  const auto actualId = client->trySend(std::vector<char>{});

  EXPECT_FALSE(actualId.has_value());
}

TEST_F(Integration_Net_Client_AsioClient, WritesDataToServerSocket)
{
  auto bus    = std::make_shared<TestNetworkEventQueue>();
  auto client = std::make_shared<AsioClient>(bus);
  client->connect(this->asioContext(), LOCALHOST_URL, this->port());

  const auto sockets = this->waitForConnectionEstablishedEvent(bus);

  std::string data("test");
  client->trySend(std::vector<char>(data.begin(), data.end()));

  const auto actual = sockets.readServer(data.size());
  EXPECT_EQ("test", actual);
}

TEST_F(Integration_Net_Client_AsioClient, PublishesDataSentEvent)
{
  auto bus    = std::make_shared<TestNetworkEventQueue>();
  auto client = std::make_shared<AsioClient>(bus);
  client->connect(this->asioContext(), LOCALHOST_URL, this->port());

  const auto sockets = this->waitForConnectionEstablishedEvent(bus);

  std::string data("test");
  const auto expectedMessageId = client->trySend(std::vector<char>(data.begin(), data.end()));
  EXPECT_TRUE(expectedMessageId.has_value());

  const auto event = bus->waitForEvent();
  EXPECT_EQ(EventType::DATA_SENT, event->type());
  EXPECT_FALSE(event->as<DataSentEvent>().tryGetClientId().has_value());
  const std::vector<char> expectedData(data.begin(), data.end());
  EXPECT_EQ(expectedMessageId.value(), event->as<DataSentEvent>().messageId());
}

} // namespace net::details
