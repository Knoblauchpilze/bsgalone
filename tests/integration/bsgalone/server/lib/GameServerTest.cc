
#include "GameServer.hh"
#include "ClientConnectedEvent.hh"
#include "TcpFixture.hh"
#include "TestEventBus.hh"
#include <future>
#include <gtest/gtest.h>

using namespace test;

namespace bsgalone::server {
using Integration_Bsgalone_Server_GameServerTest = TcpFixture;

TEST_F(Integration_Bsgalone_Server_GameServerTest, StopsWhenRequested)
{
  auto bus = std::make_shared<TestEventBus>();
  GameServer server(bus);

  auto result = std::async(std::launch::async, [this, &server]() { server.run(this->port()); });
  bus->waitForEvent(net::EventType::SERVER_STARTED);

  server.requestStop();
  const auto event = bus->waitForEvent();
  EXPECT_EQ(net::EventType::SERVER_STOPPED, event->type());

  result.get();
}

TEST_F(Integration_Bsgalone_Server_GameServerTest, AcceptsConnectionAndPublishesEvent)
{
  auto bus = std::make_shared<TestEventBus>();
  GameServer server(bus);

  auto result = std::async(std::launch::async, [this, &server]() { server.run(this->port()); });
  bus->waitForEvent(net::EventType::SERVER_STARTED);

  const auto socket = this->connectToRunningServer();
  const auto event  = bus->waitForEvent();
  EXPECT_EQ(net::EventType::CLIENT_CONNECTED, event->type());
  EXPECT_EQ(net::ClientId{0}, event->as<net::ClientConnectedEvent>().clientId());

  server.requestStop();
  result.get();
}

} // namespace bsgalone::server
