
#include "OutputGameEventAdapter.hh"
#include "LoginMessage.hh"
#include "PlayerLoginEvent.hh"
#include "TestMessageQueue.hh"
#include <gtest/gtest.h>

using namespace test;

namespace bsgalone::server {

TEST(Unit_Bsgalone_Server_Events_OutputGameEventAdapter, ThrowsWhenOutputQueueIsNull)
{
  EXPECT_THROW([]() { OutputGameEventAdapter(nullptr); }(), std::invalid_argument);
}

TEST(Unit_Bsgalone_Server_Events_OutputGameEventAdapter, ForwardsFailedLoginMessage)
{
  auto queue = std::make_shared<TestMessageQueue>();
  OutputGameEventAdapter adapter(queue);

  core::PlayerLoginEvent event(net::ClientId{12});

  adapter.onEventReceived(event);

  EXPECT_EQ(1u, queue->messages().size());
  EXPECT_EQ(core::MessageType::LOGIN, queue->messages().at(0)->type());
  const auto &actual = queue->messages().at(0)->as<core::LoginMessage>();
  EXPECT_EQ(net::ClientId{12}, actual.getClientId());
  EXPECT_FALSE(actual.successfullyLoggedIn());
}

TEST(Unit_Bsgalone_Server_Events_OutputGameEventAdapter, ForwardsSuccessfulLoginMessage)
{
  auto queue = std::make_shared<TestMessageQueue>();
  OutputGameEventAdapter adapter(queue);

  core::PlayerLoginEvent event(net::ClientId{12});
  event.setPlayerDbId(core::Uuid{18});
  event.setRole(core::GameRole::GUNNER);
  event.setSystemDbId(core::Uuid{19});

  adapter.onEventReceived(event);

  EXPECT_EQ(1u, queue->messages().size());
  EXPECT_EQ(core::MessageType::LOGIN, queue->messages().at(0)->type());
  const auto &actual = queue->messages().at(0)->as<core::LoginMessage>();
  EXPECT_EQ(net::ClientId{12}, actual.getClientId());
  EXPECT_TRUE(actual.successfullyLoggedIn());
  EXPECT_EQ(core::Uuid{18}, actual.getPlayerDbId());
  EXPECT_EQ(core::GameRole::GUNNER, actual.getRole());
  EXPECT_EQ(core::Uuid{19}, actual.getSystemDbId());
}

} // namespace bsgalone::server
