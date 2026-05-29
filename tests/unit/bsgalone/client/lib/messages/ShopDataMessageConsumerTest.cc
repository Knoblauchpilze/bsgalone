
#include "ShopDataMessageConsumer.hh"
#include "MockDataStore.hh"
#include "ShopDataMessage.hh"
#include "TestUiEventQueue.hh"
#include <gtest/gtest.h>

using namespace test;
using namespace ::testing;

namespace bsgalone::client {

TEST(Unit_Bsgalone_Client_Messages_ShopDataMessageConsumer, ThrowsWhenDataStoreIsNull)
{
  EXPECT_THROW([]() { ShopDataMessageConsumer(nullptr, std::make_shared<TestUiEventQueue>()); }(),
               std::invalid_argument);
}

TEST(Unit_Bsgalone_Client_Messages_ShopDataMessageConsumer, ThrowsWhenEventQueueIsNull)
{
  EXPECT_THROW([]() { ShopDataMessageConsumer(std::make_shared<MockDataStore>(), nullptr); }(),
               std::invalid_argument);
}

TEST(Unit_Bsgalone_Client_Messages_ShopDataMessageConsumer, ConsidersShopDataMessageAsRelevant)
{
  ShopDataMessageConsumer consumer(std::make_shared<MockDataStore>(),
                                   std::make_shared<TestUiEventQueue>());

  EXPECT_TRUE(consumer.isEventRelevant(core::MessageType::SHOP_DATA));
}

TEST(Unit_Bsgalone_Client_Messages_ShopDataMessageConsumer,
     DoesNotPublishEventWhenPlayerIsNotLoggedIn)
{
  auto mockStore = std::make_shared<StrictMock<MockDataStore>>();
  EXPECT_CALL(*mockStore, isLoggedIn()).Times(1).WillOnce(Return(false));

  auto queue = std::make_shared<TestUiEventQueue>();

  const core::Uuid playerDbId;
  core::ShopDataMessage message(playerDbId);

  ShopDataMessageConsumer consumer(mockStore, queue);
  consumer.onEventReceived(message);

  EXPECT_TRUE(queue->empty());
}

TEST(Unit_Bsgalone_Client_Messages_ShopDataMessageConsumer,
     DoesNotPublishEventWhenMessageIsForAnotherPlayer)
{
  auto mockStore = std::make_shared<StrictMock<MockDataStore>>();
  EXPECT_CALL(*mockStore, isLoggedIn()).Times(1).WillOnce(Return(true));
  EXPECT_CALL(*mockStore, getPlayerDbId()).Times(1).WillOnce(Return(core::Uuid{}));

  auto queue = std::make_shared<TestUiEventQueue>();

  const core::Uuid playerDbId;
  core::ShopDataMessage message(playerDbId);

  ShopDataMessageConsumer consumer(mockStore, queue);
  consumer.onEventReceived(message);

  EXPECT_TRUE(queue->empty());
}

TEST(Unit_Bsgalone_Client_Messages_ShopDataMessageConsumer,
     PublishesShopReadyEventWhenMessageIsForCurrentPlayer)
{
  const core::Uuid playerDbId;
  core::ShopDataMessage message(playerDbId);

  auto mockStore = std::make_shared<StrictMock<MockDataStore>>();
  EXPECT_CALL(*mockStore, isLoggedIn()).Times(1).WillOnce(Return(true));
  EXPECT_CALL(*mockStore, getPlayerDbId()).Times(1).WillOnce(Return(playerDbId));

  auto queue = std::make_shared<TestUiEventQueue>();

  ShopDataMessageConsumer consumer(mockStore, queue);
  consumer.onEventReceived(message);

  EXPECT_EQ(1u, queue->messages().size());
  EXPECT_EQ(UiEventType::SHOP_READY, queue->messages().at(0)->type());
}

} // namespace bsgalone::client
