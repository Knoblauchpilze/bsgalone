
#include "NetworkEventListener.hh"
#include "ConnectionEstablishedEvent.hh"
#include "ConnectionLostEvent.hh"
#include "TestMessageQueue.hh"
#include <gtest/gtest.h>

using namespace test;
using namespace ::testing;

namespace bsgalone::client {

TEST(Unit_Bsgalone_Client_Network_NetworkEventListener, ThrowsWhenInputQueueIsNull)
{
  std::atomic_bool connected;
  EXPECT_THROW([&connected]() { NetworkEventListener(connected, nullptr); }(),
               std::invalid_argument);
}

TEST(Unit_Bsgalone_Client_Network_NetworkEventListener,
     ConsidersConnectionEstablishedEventAsRelevant)
{
  std::atomic_bool connected;
  NetworkEventListener listener(connected, std::make_shared<TestMessageQueue>());
  EXPECT_TRUE(listener.isEventRelevant(net::NetworkEventType::CONNECTION_ESTABLISHED));
}

TEST(Unit_Bsgalone_Client_Network_NetworkEventListener, ConsidersConnectionLostEventAsRelevant)
{
  std::atomic_bool connected;
  NetworkEventListener listener(connected, std::make_shared<TestMessageQueue>());
  EXPECT_TRUE(listener.isEventRelevant(net::NetworkEventType::CONNECTION_LOST));
}

TEST(Unit_Bsgalone_Client_Network_NetworkEventListener,
     SetsConnectedToTrueWhenServerStartedEventReceived)
{
  std::atomic_bool connected(false);
  NetworkEventListener listener(connected, std::make_shared<TestMessageQueue>());

  net::ConnectionEstablishedEvent event;
  listener.onEventReceived(event);

  EXPECT_TRUE(connected.load());
}

TEST(Unit_Bsgalone_Client_Network_NetworkEventListener,
     PublishesConnectionMessageWhenConnectionIsEstablished)
{
  std::atomic_bool connected(false);
  auto queue = std::make_shared<TestMessageQueue>();
  NetworkEventListener listener(connected, queue);

  net::ConnectionEstablishedEvent event;
  listener.onEventReceived(event);

  EXPECT_EQ(1u, queue->messages().size());
  const auto &actual = queue->messages().at(0);
  EXPECT_EQ(core::MessageType::CONNECTION, actual->type());
}

TEST(Unit_Bsgalone_Client_Network_NetworkEventListener,
     SetsConnectedToFalseWhenServerStoppedEventReceived)
{
  std::atomic_bool connected(true);
  NetworkEventListener listener(connected, std::make_shared<TestMessageQueue>());

  net::ConnectionLostEvent event;
  listener.onEventReceived(event);

  EXPECT_FALSE(connected.load());
}

} // namespace bsgalone::client
