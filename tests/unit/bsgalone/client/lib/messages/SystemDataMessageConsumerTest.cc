
#include "SystemDataMessageConsumer.hh"
#include "MockGame.hh"
#include "TestMessageFactory.hh"
#include "TestUiEventQueue.hh"
#include <gtest/gtest.h>

using namespace test;
using namespace ::testing;

namespace bsgalone::client {

TEST(Unit_Bsgalone_Client_Messages_SystemDataMessageConsumer, ThrowsWhenGameIsNull)
{
  EXPECT_THROW([]() { SystemDataMessageConsumer(nullptr, std::make_shared<TestUiEventQueue>()); }(),
               std::invalid_argument);
}

TEST(Unit_Bsgalone_Client_Messages_SystemDataMessageConsumer, ThrowsWhenEventQueueIsNull)
{
  EXPECT_THROW([]() { SystemDataMessageConsumer(std::make_shared<MockGame>(), nullptr); }(),
               std::invalid_argument);
}

TEST(Unit_Bsgalone_Client_Messages_SystemDataMessageConsumer, ConsidersSystemDataMessageAsRelevant)
{
  SystemDataMessageConsumer consumer(std::make_shared<MockGame>(),
                                     std::make_shared<TestUiEventQueue>());

  EXPECT_TRUE(consumer.isEventRelevant(core::MessageType::SYSTEM_DATA));
}

TEST(Unit_Bsgalone_Client_Messages_SystemDataMessageConsumer, ForwardsSystemDataToGame)
{
  auto message = generateSystemDataMessage({});

  auto mockGame = std::make_shared<StrictMock<MockGame>>();
  EXPECT_CALL(*mockGame,
              onSystemDataReceived(SystemData{
                .name        = message.getSystemName(),
                .currentTick = message.getCurrentTick(),
                .step        = message.getTimeStep(),

              }))
    .Times(1);

  SystemDataMessageConsumer consumer(std::move(mockGame), std::make_shared<TestUiEventQueue>());

  consumer.onEventReceived(message);
}

TEST(Unit_Bsgalone_Client_Messages_SystemDataMessageConsumer, PublishesLoadingFinishedEvent)
{
  auto queue = std::make_shared<TestUiEventQueue>();
  SystemDataMessageConsumer consumer(std::make_shared<MockGame>(), queue);

  auto message = generateSystemDataMessage({});
  consumer.onEventReceived(message);

  EXPECT_EQ(1u, queue->messages().size());
  EXPECT_EQ(UiEventType::GAME_READY, queue->messages().at(0)->type());
}

} // namespace bsgalone::client
