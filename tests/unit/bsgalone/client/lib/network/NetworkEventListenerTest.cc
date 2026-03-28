
#include "NetworkEventListener.hh"
#include "ConnectionEstablishedEvent.hh"
#include "ConnectionLostEvent.hh"
#include <gtest/gtest.h>

namespace bsgalone::client {
TEST(Unit_Bsgalone_Client_Network_NetworkEventListener,
     ConsidersConnectionEstablishedEventAsRelevant)
{
  std::atomic_bool connected;
  NetworkEventListener listener(connected);

  EXPECT_TRUE(listener.isEventRelevant(net::NetworkEventType::CONNECTION_ESTABLISHED));
}

TEST(Unit_Bsgalone_Client_Network_NetworkEventListener, ConsidersConnectionLostEventAsRelevant)
{
  std::atomic_bool connected;
  NetworkEventListener listener(connected);

  EXPECT_TRUE(listener.isEventRelevant(net::NetworkEventType::CONNECTION_LOST));
}

TEST(Unit_Bsgalone_Client_Network_NetworkEventListener,
     SetsConnectedToTrueWhenServerStartedEventReceived)
{
  std::atomic_bool connected(false);
  NetworkEventListener listener(connected);

  net::ConnectionEstablishedEvent event;
  listener.onEventReceived(event);

  EXPECT_TRUE(connected.load());
}

TEST(Unit_Bsgalone_Client_Network_NetworkEventListener,
     SetsConnectedToFalseWhenServerStoppedEventReceived)
{
  std::atomic_bool connected(true);
  NetworkEventListener listener(connected);

  net::ConnectionLostEvent event;
  listener.onEventReceived(event);

  EXPECT_FALSE(connected.load());
}

} // namespace bsgalone::client
