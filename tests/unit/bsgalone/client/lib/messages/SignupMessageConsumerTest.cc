
#include "SignupMessageConsumer.hh"
#include "SignupMessage.hh"
#include "TestUiEventQueue.hh"
#include <gtest/gtest.h>

using namespace test;
using namespace ::testing;

namespace bsgalone::client {

TEST(Unit_Bsgalone_Client_Messages_SignupMessageConsumer, ThrowsWhenEventQueueIsNull)
{
  EXPECT_THROW([]() { SignupMessageConsumer(nullptr); }(), std::invalid_argument);
}

TEST(Unit_Bsgalone_Client_Messages_SignupMessageConsumer, ConsidersSignupMessageAsRelevant)
{
  SignupMessageConsumer consumer(std::make_shared<TestUiEventQueue>());

  EXPECT_TRUE(consumer.isEventRelevant(core::MessageType::SIGNUP));
}

TEST(Unit_Bsgalone_Client_Messages_SignupMessageConsumer, PublishesSignupFailedEventWhenSignupFails)
{
  auto queue = std::make_shared<TestUiEventQueue>();
  SignupMessageConsumer consumer(queue);

  core::SignupMessage message(false);
  consumer.onEventReceived(message);

  EXPECT_EQ(1u, queue->messages().size());
  EXPECT_EQ(UiEventType::SIGNUP_FAILED, queue->messages().at(0)->type());
}

TEST(Unit_Bsgalone_Client_Messages_SignupMessageConsumer,
     PublishesSignupSucceededEventWhenSignupSucceeds)
{
  auto queue = std::make_shared<TestUiEventQueue>();
  SignupMessageConsumer consumer(queue);

  core::SignupMessage message(true);
  consumer.onEventReceived(message);

  EXPECT_EQ(1u, queue->messages().size());
  EXPECT_EQ(UiEventType::SIGNUP_SUCCEEDED, queue->messages().at(0)->type());
}

} // namespace bsgalone::client
