
#include "ClientEventListener.hh"
#include "ClientConnectedEvent.hh"
#include "ClientDisconnectedEvent.hh"
#include "LogoutMessage.hh"
#include "TestMessageQueue.hh"
#include <gtest/gtest.h>

using namespace test;
using namespace ::testing;

namespace bsgalone::server {

TEST(Unit_Bsgalone_Server_Events_ClientEventListener, ThrowsWhenClientManagerIsNull)
{
  EXPECT_THROW([]() { ClientEventListener(nullptr, std::make_shared<TestMessageQueue>()); }(),
               std::invalid_argument);
}

TEST(Unit_Bsgalone_Server_Events_ClientEventListener, ThrowsWhenInputQueueIsNull)
{
  EXPECT_THROW([]() { ClientEventListener(std::make_shared<ClientManager>(), nullptr); }(),
               std::invalid_argument);
}

TEST(Unit_Bsgalone_Server_Events_ClientEventListener, ConsidersClientConnectedEventAsRelevant)
{
  ClientEventListener listener(std::make_shared<ClientManager>(),
                               std::make_shared<TestMessageQueue>());
  EXPECT_TRUE(listener.isEventRelevant(net::EventType::CLIENT_CONNECTED));
}

TEST(Unit_Bsgalone_Server_Events_ClientEventListener, ConsidersClientDisconnectedEventAsRelevant)
{
  ClientEventListener listener(std::make_shared<ClientManager>(),
                               std::make_shared<TestMessageQueue>());
  EXPECT_TRUE(listener.isEventRelevant(net::EventType::CLIENT_DISCONNECTED));
}

TEST(Unit_Bsgalone_Server_Events_ClientEventListener,
     RegistersClientWhenClientConnectedEventIsReceived)
{
  auto manager = std::make_shared<ClientManager>();
  ClientEventListener listener(manager, std::make_shared<TestMessageQueue>());

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
  ClientEventListener listener(manager, std::make_shared<TestMessageQueue>());

  net::ClientDisconnectedEvent event(net::ClientId{12});
  listener.onEventReceived(event);

  EXPECT_EQ(std::vector<net::ClientId>(), manager->getAllClients());
}

TEST(Unit_Bsgalone_Server_Events_ClientEventListener,
     PublishesLogoutMessageWgenClientDisconnectedEventIsReceived)
{
  auto manager = std::make_shared<ClientManager>();
  manager->registerClient(net::ClientId{12});
  manager->registerPlayer(net::ClientId{12}, bsgo::Uuid{17}, bsgo::Uuid{18});

  auto queue = std::make_shared<TestMessageQueue>();
  ClientEventListener listener(manager, queue);

  net::ClientDisconnectedEvent event(net::ClientId{12});
  listener.onEventReceived(event);

  EXPECT_EQ(1u, queue->messages().size());
  const auto &actual = queue->messages().at(0);
  EXPECT_EQ(bsgalone::core::MessageType::LOGOUT, actual->type());
  EXPECT_EQ(net::ClientId{12}, actual->as<bsgo::LogoutMessage>().getClientId());
  EXPECT_EQ(bsgo::Uuid{17}, actual->as<bsgo::LogoutMessage>().getPlayerDbId());
  EXPECT_TRUE(actual->as<bsgo::LogoutMessage>().shouldCloseConnection());
}

TEST(Unit_Bsgalone_Server_Events_ClientEventListener,
     DoesNotPublishLogoutMessageWgenClientIsNotLoggedIn)
{
  auto manager = std::make_shared<ClientManager>();
  manager->registerClient(net::ClientId{12});

  auto queue = std::make_shared<TestMessageQueue>();
  ClientEventListener listener(manager, queue);

  net::ClientDisconnectedEvent event(net::ClientId{12});
  listener.onEventReceived(event);

  EXPECT_TRUE(queue->empty());
}

} // namespace bsgalone::server
