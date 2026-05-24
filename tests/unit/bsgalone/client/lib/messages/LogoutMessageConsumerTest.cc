
#include "LogoutMessageConsumer.hh"
#include "LogoutMessage.hh"
#include "MockDataStore.hh"
#include "TestUiEventQueue.hh"
#include <gtest/gtest.h>

using namespace test;
using namespace ::testing;

namespace bsgalone::client {

TEST(Unit_Bsgalone_Client_Messages_LogoutMessageConsumer, ThrowsWhenDataStoreIsNull)
{
  EXPECT_THROW([]() { LogoutMessageConsumer(nullptr, std::make_shared<TestUiEventQueue>()); }(),
               std::invalid_argument);
}

TEST(Unit_Bsgalone_Client_Messages_LogoutMessageConsumer, ThrowsWhenEventQueueIsNull)
{
  EXPECT_THROW([]() { LogoutMessageConsumer(std::make_shared<MockDataStore>(), nullptr); }(),
               std::invalid_argument);
}

TEST(Unit_Bsgalone_Client_Messages_LogoutMessageConsumer, ConsidersLogoutMessageAsRelevant)
{
  LogoutMessageConsumer consumer(std::make_shared<MockDataStore>(),
                                 std::make_shared<TestUiEventQueue>());

  EXPECT_TRUE(consumer.isEventRelevant(core::MessageType::LOGOUT));
}

TEST(Unit_Bsgalone_Client_Messages_LogoutMessageConsumer, ChecksLoginStatusFromStore)
{
  auto mockStore = std::make_shared<NiceMock<MockDataStore>>();
  EXPECT_CALL(*mockStore, isLoggedIn()).Times(1).WillOnce(Return(false));

  LogoutMessageConsumer consumer(mockStore, std::make_shared<TestUiEventQueue>());

  core::LogoutMessage message(core::Uuid{});

  consumer.onEventReceived(message);
}

TEST(Unit_Bsgalone_Client_Messages_LogoutMessageConsumer,
     ChecksPlayerIdentifierWhenStatusIndicatesLogin)
{
  const core::Uuid playerDbId;

  auto mockStore = std::make_shared<NiceMock<MockDataStore>>();
  EXPECT_CALL(*mockStore, isLoggedIn()).Times(1).WillOnce(Return(true));
  EXPECT_CALL(*mockStore, getPlayerDbId()).Times(1).WillOnce(Return(playerDbId));

  LogoutMessageConsumer consumer(mockStore, std::make_shared<TestUiEventQueue>());

  const core::Uuid playerDbId2;
  core::LogoutMessage message(playerDbId2);

  consumer.onEventReceived(message);
}

TEST(Unit_Bsgalone_Client_Messages_LogoutMessageConsumer,
     PublishesLogoutEventWhenPlayerIsNoLongerLoggedin)
{
  const core::Uuid playerDbId;

  auto mockStore = std::make_shared<StrictMock<MockDataStore>>();
  EXPECT_CALL(*mockStore, isLoggedIn()).Times(1).WillOnce(Return(true));
  EXPECT_CALL(*mockStore, getPlayerDbId()).Times(1).WillOnce(Return(playerDbId));

  EXPECT_CALL(*mockStore, onPlayerLoggedOut()).Times(1);

  auto queue = std::make_shared<TestUiEventQueue>();
  LogoutMessageConsumer consumer(mockStore, queue);

  core::LogoutMessage message(playerDbId);
  consumer.onEventReceived(message);

  EXPECT_EQ(1u, queue->messages().size());
  EXPECT_EQ(UiEventType::LOGOUT, queue->messages().at(0)->type());
}

TEST(Unit_Bsgalone_Client_Messages_LogoutMessageConsumer,
     DoesNotPublishEventWhenLogoutIsNotForCurrentPlayer)
{
  const core::Uuid playerDbId1;

  auto mockStore = std::make_shared<StrictMock<MockDataStore>>();
  EXPECT_CALL(*mockStore, isLoggedIn()).Times(1).WillOnce(Return(true));
  EXPECT_CALL(*mockStore, getPlayerDbId()).Times(1).WillOnce(Return(playerDbId1));

  const core::Uuid playerDbId2;

  auto queue = std::make_shared<TestUiEventQueue>();
  LogoutMessageConsumer consumer(mockStore, queue);

  core::LogoutMessage message(playerDbId2);
  consumer.onEventReceived(message);

  EXPECT_TRUE(queue->messages().empty());
}

} // namespace bsgalone::client
