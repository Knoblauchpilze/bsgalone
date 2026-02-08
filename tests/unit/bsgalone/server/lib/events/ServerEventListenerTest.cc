
#include "ServerEventListener.hh"
#include "ServerStartedEvent.hh"
#include "ServerStoppedEvent.hh"
#include <gtest/gtest.h>

using namespace ::testing;

namespace bsgalone::server {

TEST(Unit_Bsgalone_Server_Events_ServerEventListener, ConsidersServerStartedEventAsRelevant)
{
  std::atomic_bool started;
  ServerEventListener listener(started);
  EXPECT_TRUE(listener.isEventRelevant(net::EventType::SERVER_STARTED));
}

TEST(Unit_Bsgalone_Server_Events_ServerEventListener, ConsidersServerStoppedEventAsRelevant)
{
  std::atomic_bool started;
  ServerEventListener listener(started);
  EXPECT_TRUE(listener.isEventRelevant(net::EventType::SERVER_STOPPED));
}

TEST(Unit_Bsgalone_Server_Events_ServerEventListener,
     SetsStartedToTrueWhenServerStartedEventReceived)
{
  std::atomic_bool started(false);
  ServerEventListener listener(started);

  net::ServerStartedEvent event;
  listener.onEventReceived(event);

  EXPECT_TRUE(started.load());
}

TEST(Unit_Bsgalone_Server_Events_ServerEventListener,
     SetsStartedToFalseWhenServerStoppedEventReceived)
{
  std::atomic_bool started(true);
  ServerEventListener listener(started);

  net::ServerStoppedEvent event;
  listener.onEventReceived(event);

  EXPECT_FALSE(started.load());
}

} // namespace bsgalone::server
