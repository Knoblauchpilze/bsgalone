
#include "GameClient.hh"
#include "ClientConnectedEvent.hh"
#include "DataReceivedEvent.hh"
#include "TcpServerFixture.hh"
#include "TestNetworkEventQueue.hh"
#include <future>
#include <gtest/gtest.h>

using namespace test;

namespace bsgalone::client {
using Integration_Bsgalone_Client_GameClientTest = TcpServerFixture;

TEST_F(Integration_Bsgalone_Client_GameClientTest, ConnectsToRemoteServer)
{
  auto bus = std::make_shared<TestNetworkEventQueue>();
  GameClient client(bus);

  auto result = std::async(std::launch::async, [this, &client]() { client.run(this->port()); });
  bus->waitForEvent(net::EventType::SERVER_STARTED);

  const auto socket = this->waitForServerSocket();

  client.requestStop();
  result.get();
}

TEST_F(Integration_Bsgalone_Client_GameClientTest, StopsWhenRequested)
{
  auto bus = std::make_shared<TestNetworkEventQueue>();
  GameClient client(bus);

  auto result = std::async(std::launch::async, [this, &client]() { client.run(this->port()); });
  bus->waitForEvent(net::EventType::SERVER_STARTED);

  client.requestStop();
  // It is expected to receive a client connected event, a client disconnected event
  // and finally the server stopped event.
  bus->waitForEvent(net::EventType::SERVER_STOPPED, 3);

  result.get();
}

TEST_F(Integration_Bsgalone_Client_GameClientTest, PublishesEventWhenServerSendsData)
{
  auto bus = std::make_shared<TestNetworkEventQueue>();
  GameClient client(bus);

  auto result = std::async(std::launch::async, [this, &client]() { client.run(this->port()); });
  bus->waitForEvent(net::EventType::SERVER_STARTED);

  const auto sockets = this->waitForConnectionEstablishedEvent(bus);

  std::string data("test");
  sockets.writeServer(data);

  const auto event = bus->waitForEvent();
  EXPECT_EQ(net::EventType::DATA_RECEIVED, event->type());
  EXPECT_FALSE(event->as<net::DataReceivedEvent>().tryGetClientId().has_value());
  const std::vector<char> expected(data.begin(), data.end());
  EXPECT_EQ(expected, event->as<net::DataReceivedEvent>().data());

  client.requestStop();
  result.get();
}

} // namespace bsgalone::client
