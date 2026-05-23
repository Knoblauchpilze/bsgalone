
#include "SystemDataMessageConsumer.hh"
#include "MockGame.hh"
#include "TestMessageFactory.hh"
#include <gtest/gtest.h>

using namespace test;
using namespace ::testing;

namespace bsgalone::client {

TEST(Unit_Bsgalone_Client_Messages_SystemDataMessageConsumer, ThrowsWhenGameIsNull)
{
  EXPECT_THROW([]() { SystemDataMessageConsumer(nullptr); }(), std::invalid_argument);
}

TEST(Unit_Bsgalone_Client_Messages_SystemDataMessageConsumer, ConsidersSystemDataMessageAsRelevant)
{
  SystemDataMessageConsumer consumer(std::make_shared<MockGame>());

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

  SystemDataMessageConsumer consumer(std::move(mockGame));

  consumer.onEventReceived(message);
}

} // namespace bsgalone::client
