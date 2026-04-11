
#include "LoginMessageConsumer.hh"
#include "LoginMessage.hh"
#include "MockDataStore.hh"
#include "TestUiEventQueue.hh"
#include <gtest/gtest.h>

using namespace test;
using namespace ::testing;

namespace bsgalone::client {

TEST(Unit_Bsgalone_Client_Messages_LoginMessageConsumer, ThrowsWhenDataStoreIsNull)
{
  EXPECT_THROW([]() { LoginMessageConsumer(nullptr, std::make_shared<TestUiEventQueue>()); }(),
               std::invalid_argument);
}

TEST(Unit_Bsgalone_Client_Messages_LoginMessageConsumer, ThrowsWhenEventQueueIsNull)
{
  EXPECT_THROW([]() { LoginMessageConsumer(std::make_shared<MockDataStore>(), nullptr); }(),
               std::invalid_argument);
}

TEST(Unit_Bsgalone_Client_Messages_LoginMessageConsumer, ConsidersLoginMessageAsRelevant)
{
  LoginMessageConsumer consumer(std::make_shared<MockDataStore>(),
                                std::make_shared<TestUiEventQueue>());

  EXPECT_TRUE(consumer.isEventRelevant(core::MessageType::LOGIN));
}

TEST(Unit_Bsgalone_Client_Messages_LoginMessageConsumer, DoesNotCallStoreWhenLoginFailed)
{
  // Creating a strict mock will fail if any call are detected
  // on it which should not happen because the consumer should
  // not forward the call.
  auto mockStore = std::make_shared<StrictMock<MockDataStore>>();

  LoginMessageConsumer consumer(mockStore, std::make_shared<TestUiEventQueue>());

  core::LoginMessage message;
  consumer.onEventReceived(message);
}

TEST(Unit_Bsgalone_Client_Messages_LoginMessageConsumer, PublishesLoginFailedEventWhenLoginFails)
{
  auto queue = std::make_shared<TestUiEventQueue>();
  LoginMessageConsumer consumer(std::make_shared<MockDataStore>(), queue);

  core::LoginMessage message;
  consumer.onEventReceived(message);

  EXPECT_EQ(1u, queue->messages().size());
  EXPECT_EQ(UiEventType::LOGIN_FAILED, queue->messages().at(0)->type());
}

TEST(Unit_Bsgalone_Client_Messages_LoginMessageConsumer, ForwardsToStoreWhenLoginSucceeded)
{
  auto mockStore = std::make_shared<StrictMock<MockDataStore>>();
  EXPECT_CALL(*mockStore, onPlayerLoggedIn(core::Uuid{18}, core::GameRole::PILOT)).Times(1);

  LoginMessageConsumer consumer(mockStore, std::make_shared<TestUiEventQueue>());

  core::LoginMessage message;
  message.setPlayerDbId(core::Uuid{18});
  message.setRole(core::GameRole::PILOT);

  consumer.onEventReceived(message);
}

TEST(Unit_Bsgalone_Client_Messages_LoginMessageConsumer,
     PublishesLoginSucceededEventWhenLoginSucceeds)
{
  auto queue = std::make_shared<TestUiEventQueue>();
  LoginMessageConsumer consumer(std::make_shared<MockDataStore>(), queue);

  core::LoginMessage message;
  message.setPlayerDbId(core::Uuid{18});
  message.setRole(core::GameRole::PILOT);
  consumer.onEventReceived(message);

  EXPECT_EQ(1u, queue->messages().size());
  EXPECT_EQ(UiEventType::LOGIN_SUCCEEDED, queue->messages().at(0)->type());
}

} // namespace bsgalone::client
