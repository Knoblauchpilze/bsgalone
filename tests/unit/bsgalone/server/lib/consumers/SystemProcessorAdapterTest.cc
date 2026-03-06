
#include "SystemProcessorAdapter.hh"
#include "ShipSelectedMessage.hh"
#include "TestMessageQueue.hh"
#include <gtest/gtest.h>

using namespace test;
using namespace ::testing;

namespace bsgalone::server {

TEST(Unit_Bsgalone_Server_Consumers_SystemProcessorAdapter, ThrowsWhenInputQueueIsNull)
{
  EXPECT_THROW([this]() { SystemProcessorAdapter(core::Uuid{19}, nullptr); }(),
               std::invalid_argument);
}

TEST(Unit_Bsgalone_Server_Consumers_SystemProcessorAdapter, ForwardsMessageWhenDirectedToSystem)
{
  auto queue = std::make_shared<TestMessageQueue>();
  SystemProcessorAdapter adapter(core::Uuid{19}, queue);

  core::ShipSelectedMessage message(core::Uuid{18}, core::Uuid{19}, core::Uuid{7});
  adapter.onEventReceived(message);

  EXPECT_EQ(1u, queue->messages().size());
  const auto &actual = queue->messages().at(0);
  EXPECT_EQ(core::MessageType::SHIP_SELECTED, actual->type());
  EXPECT_EQ(core::Uuid{18}, actual->as<core::ShipSelectedMessage>().getPlayerDbId());
  EXPECT_EQ(core::Uuid{19}, actual->as<core::ShipSelectedMessage>().getSystemDbId());
  EXPECT_EQ(core::Uuid{7}, actual->as<core::ShipSelectedMessage>().getShipDbId());
}

TEST(Unit_Bsgalone_Server_Consumers_SystemProcessorAdapter,
     DoesNotForwardMessageWhenNotDirectedToSystem)
{
  auto queue = std::make_shared<TestMessageQueue>();
  SystemProcessorAdapter adapter(core::Uuid{19}, queue);

  core::ShipSelectedMessage message(core::Uuid{18}, core::Uuid{20}, core::Uuid{7});
  adapter.onEventReceived(message);

  EXPECT_TRUE(queue->empty());
}

} // namespace bsgalone::server
