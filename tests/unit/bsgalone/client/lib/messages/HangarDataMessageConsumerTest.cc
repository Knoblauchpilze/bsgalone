
#include "HangarDataMessageConsumer.hh"
#include "HangarDataMessage.hh"
#include "MockDataStore.hh"
#include "TestUiEventQueue.hh"
#include <gtest/gtest.h>

using namespace test;
using namespace ::testing;

namespace bsgalone::client {

TEST(Unit_Bsgalone_Client_Messages_HangarDataMessageConsumer, ThrowsWhenDataStoreIsNull)
{
  EXPECT_THROW([]() { HangarDataMessageConsumer(nullptr, std::make_shared<TestUiEventQueue>()); }(),
               std::invalid_argument);
}

TEST(Unit_Bsgalone_Client_Messages_HangarDataMessageConsumer, ThrowsWhenEventQueueIsNull)
{
  EXPECT_THROW([]() { HangarDataMessageConsumer(std::make_shared<MockDataStore>(), nullptr); }(),
               std::invalid_argument);
}

TEST(Unit_Bsgalone_Client_Messages_HangarDataMessageConsumer, ConsidersHangarDataMessageAsRelevant)
{
  HangarDataMessageConsumer consumer(std::make_shared<MockDataStore>(),
                                     std::make_shared<TestUiEventQueue>());

  EXPECT_TRUE(consumer.isEventRelevant(core::MessageType::HANGAR_DATA));
}

TEST(Unit_Bsgalone_Client_Messages_HangarDataMessageConsumer,
     DoesNotPublishEventWhenPlayerIsNotLoggedIn)
{
  auto mockStore = std::make_shared<StrictMock<MockDataStore>>();
  EXPECT_CALL(*mockStore, isLoggedIn()).Times(1).WillOnce(Return(false));

  auto queue = std::make_shared<TestUiEventQueue>();

  const core::Uuid playerDbId;
  core::HangarDataMessage message(playerDbId);

  HangarDataMessageConsumer consumer(mockStore, queue);
  consumer.onEventReceived(message);

  EXPECT_TRUE(queue->empty());
}

TEST(Unit_Bsgalone_Client_Messages_HangarDataMessageConsumer,
     DoesNotPublishEventWhenMessageIsForAnotherPlayer)
{
  auto mockStore = std::make_shared<StrictMock<MockDataStore>>();
  EXPECT_CALL(*mockStore, isLoggedIn()).Times(1).WillOnce(Return(true));
  EXPECT_CALL(*mockStore, getPlayerDbId()).Times(1).WillOnce(Return(core::Uuid{}));

  auto queue = std::make_shared<TestUiEventQueue>();

  const core::Uuid playerDbId;
  core::HangarDataMessage message(playerDbId);

  HangarDataMessageConsumer consumer(mockStore, queue);
  consumer.onEventReceived(message);

  EXPECT_TRUE(queue->empty());
}

TEST(Unit_Bsgalone_Client_Messages_HangarDataMessageConsumer,
     PublishesHangarReadyEventWhenMessageIsForCurrentPlayer)
{
  const core::Uuid playerDbId;
  core::HangarDataMessage message(playerDbId);

  auto mockStore = std::make_shared<StrictMock<MockDataStore>>();
  EXPECT_CALL(*mockStore, isLoggedIn()).Times(1).WillOnce(Return(true));
  EXPECT_CALL(*mockStore, getPlayerDbId()).Times(1).WillOnce(Return(playerDbId));

  auto queue = std::make_shared<TestUiEventQueue>();

  HangarDataMessageConsumer consumer(mockStore, queue);
  consumer.onEventReceived(message);

  EXPECT_EQ(1u, queue->messages().size());
  EXPECT_EQ(UiEventType::HANGAR_READY, queue->messages().at(0)->type());
}

} // namespace bsgalone::client
