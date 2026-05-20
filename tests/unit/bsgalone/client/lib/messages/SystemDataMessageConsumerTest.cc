
#include "SystemDataMessageConsumer.hh"
#include "MockDataStore.hh"
#include "SystemDataMessage.hh"
#include <gtest/gtest.h>

using namespace test;
using namespace ::testing;

namespace bsgalone::client {

TEST(Unit_Bsgalone_Client_Messages_SystemDataMessageConsumer, ThrowsWhenDataStoreIsNull)
{
  EXPECT_THROW([]() { SystemDataMessageConsumer(nullptr); }(), std::invalid_argument);
}

TEST(Unit_Bsgalone_Client_Messages_SystemDataMessageConsumer, ConsidersSystemDataMessageAsRelevant)
{
  SystemDataMessageConsumer consumer(std::make_shared<MockDataStore>());

  EXPECT_TRUE(consumer.isEventRelevant(core::MessageType::SYSTEM_DATA));
}

} // namespace bsgalone::client
