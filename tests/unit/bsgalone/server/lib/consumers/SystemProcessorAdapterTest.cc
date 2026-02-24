
#include "SystemProcessorAdapter.hh"
#include "ShipSelectedMessage.hh"
#include "TestMessageQueue.hh"
#include <gtest/gtest.h>

using namespace test;
using namespace ::testing;

namespace bsgalone::server {

TEST(Unit_Bsgalone_Server_Consumers_SystemProcessorAdapter, ForwardsMessageWhenDirectedToSystem)
{
  auto queue = std::make_shared<TestMessageQueue>();
  SystemProcessorAdapter adapter(bsgo::Uuid{19}, queue);

  core::ShipSelectedMessage message(bsgo::Uuid{18}, bsgo::Uuid{19}, bsgo::Uuid{7});
  adapter.onMessageReceived(message);

  EXPECT_EQ(1u, queue->messages().size());
  const auto &actual = queue->messages().at(0);
  EXPECT_EQ(core::MessageType::SHIP_SELECTED, actual->type());
  EXPECT_EQ(bsgo::Uuid{18}, actual->as<core::ShipSelectedMessage>().getPlayerDbId());
  EXPECT_EQ(bsgo::Uuid{19}, actual->as<core::ShipSelectedMessage>().getSystemDbId());
  EXPECT_EQ(bsgo::Uuid{7}, actual->as<core::ShipSelectedMessage>().getShipDbId());
}

TEST(Unit_Bsgalone_Server_Consumers_SystemProcessorAdapter,
     DoesNotForwardMessageWhenNotDirectedToSystem)
{
  auto queue = std::make_shared<TestMessageQueue>();
  SystemProcessorAdapter adapter(bsgo::Uuid{19}, queue);

  core::ShipSelectedMessage message(bsgo::Uuid{18}, bsgo::Uuid{20}, bsgo::Uuid{7});
  adapter.onMessageReceived(message);

  EXPECT_TRUE(queue->empty());
}

} // namespace bsgalone::server
