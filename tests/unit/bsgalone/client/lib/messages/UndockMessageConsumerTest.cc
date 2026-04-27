
#include "UndockMessageConsumer.hh"
#include "MockDataStore.hh"
#include "TestUiEventQueue.hh"
#include "UndockMessage.hh"
#include <gtest/gtest.h>

using namespace test;
using namespace ::testing;

namespace bsgalone::client {

TEST(Unit_Bsgalone_Client_Messages_UndockMessageConsumer, ThrowsWhenDataStoreIsNull)
{
  EXPECT_THROW([]() { UndockMessageConsumer(nullptr, std::make_shared<TestUiEventQueue>()); }(),
               std::invalid_argument);
}

TEST(Unit_Bsgalone_Client_Messages_UndockMessageConsumer, ThrowsWhenEventQueueIsNull)
{
  EXPECT_THROW([]() { UndockMessageConsumer(std::make_shared<MockDataStore>(), nullptr); }(),
               std::invalid_argument);
}

TEST(Unit_Bsgalone_Client_Messages_UndockMessageConsumer, ConsidersUndockMessageAsRelevant)
{
  UndockMessageConsumer consumer(std::make_shared<MockDataStore>(),
                                 std::make_shared<TestUiEventQueue>());

  EXPECT_TRUE(consumer.isEventRelevant(core::MessageType::UNDOCK));
}

TEST(Unit_Bsgalone_Client_Messages_UndockMessageConsumer, DoesNotPublishEventWhenPlayerIsNotLoggedIn)
{
  auto mockStore = std::make_shared<StrictMock<MockDataStore>>();
  EXPECT_CALL(*mockStore, isLoggedIn()).Times(1).WillOnce(Return(false));

  auto queue = std::make_shared<TestUiEventQueue>();

  const core::Uuid playerDbId;
  core::UndockMessage message(playerDbId);

  UndockMessageConsumer consumer(mockStore, queue);
  consumer.onEventReceived(message);

  EXPECT_TRUE(queue->empty());
}

TEST(Unit_Bsgalone_Client_Messages_UndockMessageConsumer,
     DoesNotPublishEventWhenMessageIsForAnotherPlayer)
{
  auto mockStore = std::make_shared<StrictMock<MockDataStore>>();
  EXPECT_CALL(*mockStore, isLoggedIn()).Times(1).WillOnce(Return(true));
  EXPECT_CALL(*mockStore, getPlayerDbId()).Times(1).WillOnce(Return(core::Uuid{}));

  auto queue = std::make_shared<TestUiEventQueue>();

  const core::Uuid playerDbId;
  core::UndockMessage message(playerDbId);

  UndockMessageConsumer consumer(mockStore, queue);
  consumer.onEventReceived(message);

  EXPECT_TRUE(queue->empty());
}

TEST(Unit_Bsgalone_Client_Messages_UndockMessageConsumer,
     PublishesUndockEventWhenMessageIsForCurrentPlayer)
{
  const core::Uuid playerDbId;
  core::UndockMessage message(playerDbId);

  auto mockStore = std::make_shared<StrictMock<MockDataStore>>();
  EXPECT_CALL(*mockStore, isLoggedIn()).Times(1).WillOnce(Return(true));
  EXPECT_CALL(*mockStore, getPlayerDbId()).Times(1).WillOnce(Return(playerDbId));

  auto queue = std::make_shared<TestUiEventQueue>();

  UndockMessageConsumer consumer(mockStore, queue);
  consumer.onEventReceived(message);

  EXPECT_EQ(1u, queue->messages().size());
  EXPECT_EQ(UiEventType::UNDOCK, queue->messages().at(0)->type());
}

} // namespace bsgalone::client
