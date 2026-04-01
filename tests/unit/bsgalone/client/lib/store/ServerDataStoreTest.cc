
#include "ServerDataStore.hh"
#include "TestUiEventQueue.hh"
#include <gtest/gtest.h>

using namespace test;
using namespace ::testing;

namespace bsgalone::client {

TEST(Unit_Bsgalone_Client_Store_ServerDataStore, ThrowsWhenEventQueueIsNull)
{
  EXPECT_THROW([]() { ServerDataStore(nullptr); }(), std::invalid_argument);
}

TEST(Unit_Bsgalone_Client_Store_ServerDataStore, PublishesLoginSucceededEventWhenDataReceived)
{
  auto queue = std::make_shared<TestUiEventQueue>();
  ServerDataStore store(queue);

  store.onPlayerLoggedIn(core::Uuid{18}, core::Uuid{19}, core::GameRole::GUNNER);

  EXPECT_EQ(1u, queue->messages().size());
  EXPECT_EQ(UiEventType::LOGIN_SUCCEEDED, queue->messages().at(0)->type());
}

} // namespace bsgalone::client
