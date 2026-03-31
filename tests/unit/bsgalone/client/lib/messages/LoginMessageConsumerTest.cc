
#include "LoginMessageConsumer.hh"
#include "LoginMessage.hh"
#include "MockDataStore.hh"
#include <gtest/gtest.h>

using namespace test;
using namespace ::testing;

namespace bsgalone::client {

TEST(Unit_Bsgalone_Client_Messages_LoginMessageConsumer, ThrowsWhenDataStoreIsNull)
{
  EXPECT_THROW([]() { LoginMessageConsumer(nullptr); }(), std::invalid_argument);
}

TEST(Unit_Bsgalone_Client_Messages_LoginMessageConsumer, ConsidersLoginMessageAsRelevant)
{
  LoginMessageConsumer consumer(std::make_shared<MockDataStore>());

  EXPECT_TRUE(consumer.isEventRelevant(core::MessageType::LOGIN));
}

TEST(Unit_Bsgalone_Client_Messages_LoginMessageConsumer, DoesNotCallStoreWhenLoginFailed)
{
  // Creating a strict mock will fail if any call are detected
  // on it which should not happen because the consumer should
  // not forward the call.
  auto mockStore = std::make_shared<StrictMock<MockDataStore>>();

  LoginMessageConsumer consumer(mockStore);

  core::LoginMessage message(net::ClientId{12});
  consumer.onEventReceived(message);
}

TEST(Unit_Bsgalone_Client_Messages_LoginMessageConsumer, ForwardsToStoreWhenLoginSucceeded)
{
  auto mockStore = std::make_shared<StrictMock<MockDataStore>>();
  EXPECT_CALL(*mockStore, onPlayerLoggedIn(core::Uuid{18}, core::Uuid{19}, core::GameRole::PILOT))
    .Times(1);

  LoginMessageConsumer consumer(mockStore);

  core::LoginMessage message(net::ClientId{12});
  message.setPlayerDbId(core::Uuid{18});
  message.setRole(core::GameRole::PILOT);
  message.setSystemDbId(core::Uuid{19});

  consumer.onEventReceived(message);
}

} // namespace bsgalone::client
