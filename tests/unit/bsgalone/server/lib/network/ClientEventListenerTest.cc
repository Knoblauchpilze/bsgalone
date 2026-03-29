
#include "ClientEventListener.hh"
#include "ClientConnectedEvent.hh"
#include "ClientDisconnectedEvent.hh"
#include "TestMessageQueue.hh"
#include <gtest/gtest.h>

using namespace test;
using namespace ::testing;

namespace bsgalone::server {

TEST(Unit_Bsgalone_Server_Events_ClientEventListener, ThrowsWhenClientManagerIsNull)
{
  EXPECT_THROW([]() { ClientEventListener(nullptr); }(), std::invalid_argument);
}

TEST(Unit_Bsgalone_Server_Events_ClientEventListener, ConsidersClientConnectedEventAsRelevant)
{
  ClientEventListener listener(std::make_shared<ClientManager>());
  EXPECT_TRUE(listener.isEventRelevant(net::NetworkEventType::CLIENT_CONNECTED));
}

TEST(Unit_Bsgalone_Server_Events_ClientEventListener, ConsidersClientDisconnectedEventAsRelevant)
{
  ClientEventListener listener(std::make_shared<ClientManager>());
  EXPECT_TRUE(listener.isEventRelevant(net::NetworkEventType::CLIENT_DISCONNECTED));
}

TEST(Unit_Bsgalone_Server_Events_ClientEventListener,
     RegistersClientWhenClientConnectedEventIsReceived)
{
  auto manager = std::make_shared<ClientManager>();
  ClientEventListener listener(manager);

  net::ClientConnectedEvent event(net::ClientId{12});
  listener.onEventReceived(event);

  const auto clients = manager->getAllClients();
  EXPECT_EQ(std::vector<net::ClientId>{net::ClientId{12}}, clients);
}

TEST(Unit_Bsgalone_Server_Events_ClientEventListener,
     RemovesClientWhenClientDisconnectedEventIsReceived)
{
  auto manager = std::make_shared<ClientManager>();
  manager->registerClient(net::ClientId{12});
  ClientEventListener listener(manager);

  net::ClientDisconnectedEvent event(net::ClientId{12});
  listener.onEventReceived(event);

  EXPECT_EQ(std::vector<net::ClientId>(), manager->getAllClients());
}

} // namespace bsgalone::server
