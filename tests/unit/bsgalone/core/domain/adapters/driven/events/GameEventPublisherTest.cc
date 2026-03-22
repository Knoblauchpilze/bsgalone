
#include "GameEventPublisher.hh"
#include "PlayerLoginEvent.hh"
#include "TestGameEventQueue.hh"
#include <gtest/gtest.h>

using namespace test;
using namespace ::testing;

namespace bsgalone::core {

TEST(Unit_Bsgalone_Core_Domain_Adapters_Driven_Events_GameEventPublisher, ThrowsWhenEventQueueIsNull)
{
  EXPECT_THROW([]() { GameEventPublisher(nullptr); }(), std::invalid_argument);
}

TEST(Unit_Bsgalone_Core_Domain_Adapters_Driven_Events_GameEventPublisher,
     ForwardsMessageToEventQueue)
{
  auto queue = std::make_shared<TestGameEventQueue>();
  GameEventPublisher publisher(queue);

  PlayerLoginEvent event(net::ClientId{12});
  publisher.publishEvent(event.clone());

  EXPECT_EQ(1u, queue->messages().size());
  EXPECT_EQ(GameEventType::PLAYER_LOGIN, queue->messages().at(0)->type());
  const auto &actual = queue->messages().at(0)->as<PlayerLoginEvent>();
  EXPECT_EQ(net::ClientId{12}, actual.getClientId());
  EXPECT_FALSE(actual.successfulLogin());
  EXPECT_FALSE(actual.tryGetPlayerDbId().has_value());
  EXPECT_FALSE(actual.tryGetRole().has_value());
  EXPECT_FALSE(actual.tryGetSystemDbId().has_value());
}

} // namespace bsgalone::core
