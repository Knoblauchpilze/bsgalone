
#include "LockerDataMessageConsumer.hh"
#include "LockerDataMessage.hh"
#include "MockDataStore.hh"
#include "TestUiEventQueue.hh"
#include <gtest/gtest.h>

using namespace test;
using namespace ::testing;

namespace bsgalone::client {

TEST(Unit_Bsgalone_Client_Messages_LockerDataMessageConsumer, ThrowsWhenDataStoreIsNull)
{
  EXPECT_THROW([]() { LockerDataMessageConsumer(nullptr, std::make_shared<TestUiEventQueue>()); }(),
               std::invalid_argument);
}

TEST(Unit_Bsgalone_Client_Messages_LockerDataMessageConsumer, ThrowsWhenEventQueueIsNull)
{
  EXPECT_THROW([]() { LockerDataMessageConsumer(std::make_shared<MockDataStore>(), nullptr); }(),
               std::invalid_argument);
}

TEST(Unit_Bsgalone_Client_Messages_LockerDataMessageConsumer, ConsidersLockerDataMessageAsRelevant)
{
  LockerDataMessageConsumer consumer(std::make_shared<MockDataStore>(),
                                     std::make_shared<TestUiEventQueue>());

  EXPECT_TRUE(consumer.isEventRelevant(core::MessageType::LOCKER_DATA));
}

TEST(Unit_Bsgalone_Client_Messages_LockerDataMessageConsumer,
     DoesNotPublishEventWhenPlayerIsNotLoggedIn)
{
  auto mockStore = std::make_shared<StrictMock<MockDataStore>>();
  EXPECT_CALL(*mockStore, isLoggedIn()).Times(1).WillOnce(Return(false));

  auto queue = std::make_shared<TestUiEventQueue>();

  const core::Uuid playerDbId;
  core::LockerDataMessage message(playerDbId);

  LockerDataMessageConsumer consumer(mockStore, queue);
  consumer.onEventReceived(message);

  EXPECT_TRUE(queue->empty());
}

TEST(Unit_Bsgalone_Client_Messages_LockerDataMessageConsumer,
     DoesNotPublishEventWhenMessageIsForAnotherPlayer)
{
  auto mockStore = std::make_shared<StrictMock<MockDataStore>>();
  EXPECT_CALL(*mockStore, isLoggedIn()).Times(1).WillOnce(Return(true));
  EXPECT_CALL(*mockStore, getPlayerDbId()).Times(1).WillOnce(Return(core::Uuid{}));

  auto queue = std::make_shared<TestUiEventQueue>();

  const core::Uuid playerDbId;
  core::LockerDataMessage message(playerDbId);

  LockerDataMessageConsumer consumer(mockStore, queue);
  consumer.onEventReceived(message);

  EXPECT_TRUE(queue->empty());
}

TEST(Unit_Bsgalone_Client_Messages_LockerDataMessageConsumer,
     PublishesLockerReadyEventWhenMessageIsForCurrentPlayer)
{
  const core::Uuid playerDbId;
  core::LockerDataMessage message(playerDbId);

  auto mockStore = std::make_shared<StrictMock<MockDataStore>>();
  EXPECT_CALL(*mockStore, isLoggedIn()).Times(1).WillOnce(Return(true));
  EXPECT_CALL(*mockStore, getPlayerDbId()).Times(1).WillOnce(Return(playerDbId));

  auto queue = std::make_shared<TestUiEventQueue>();

  LockerDataMessageConsumer consumer(mockStore, queue);
  consumer.onEventReceived(message);

  EXPECT_EQ(1u, queue->messages().size());
  EXPECT_EQ(UiEventType::LOCKER_READY, queue->messages().at(0)->type());
}

} // namespace bsgalone::client
