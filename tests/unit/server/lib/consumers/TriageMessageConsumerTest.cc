
#include "TriageMessageConsumer.hh"
#include "ClientManager.hh"
#include "SystemQueues.hh"
#include "TestMessageQueue.hh"
#include "TestPlayerMessage.hh"
#include "TestSystemMessage.hh"
#include "TriageMessageConsumer.hh"
#include <gmock/gmock-matchers.h>
#include <gtest/gtest.h>

using namespace test;
using namespace ::testing;

namespace bsgo {

TEST(Unit_Server_Consumers_TriageMessageConsumer, RoutesPlayerMessageToCorrectSystem)
{
  auto system19Queue = std::make_shared<TestMessageQueue>();
  auto system20Queue = std::make_shared<TestMessageQueue>();
  SystemQueueMap systemQueues{{Uuid{19}, system19Queue}, {Uuid{20}, system20Queue}};
  TriageMessageConsumer consumer(systemQueues);

  TestPlayerMessage message(Uuid{18}, Uuid{19});
  consumer.onMessageReceived(message);

  EXPECT_EQ(1, system19Queue->messages().size());
  const auto &actual = system19Queue->messages().at(0);
  EXPECT_EQ(bsgalone::core::MessageType::DOCK, actual->type());
  EXPECT_EQ(Uuid{18}, actual->as<TestPlayerMessage>().getPlayerDbId());
  EXPECT_EQ(Uuid{19}, actual->as<TestPlayerMessage>().getSystemDbId());

  EXPECT_TRUE(system20Queue->empty());
}

TEST(Unit_Server_Consumers_TriageMessageConsumer, ThrowsWhenPlayerMessageReferencesUnknownSystem)
{
  // Empty map to not have any available system queue
  TriageMessageConsumer consumer(SystemQueueMap{});

  EXPECT_THAT(
    [&consumer]() {
      TestPlayerMessage message(Uuid{18}, Uuid{19});
      consumer.onMessageReceived(message);
    },
    ThrowsMessage<::core::CoreException>("Failed to process message dock"));
}

TEST(Unit_Server_Consumers_TriageMessageConsumer, RoutesSystemMessageToCorrectSystem)
{
  auto system19Queue = std::make_shared<TestMessageQueue>();
  auto system20Queue = std::make_shared<TestMessageQueue>();
  SystemQueueMap systemQueues{{Uuid{19}, system19Queue}, {Uuid{20}, system20Queue}};
  TriageMessageConsumer consumer(systemQueues);

  TestSystemMessage message(Uuid{19});
  consumer.onMessageReceived(message);

  EXPECT_EQ(1, system19Queue->messages().size());
  const auto &actual = system19Queue->messages().at(0);
  EXPECT_EQ(bsgalone::core::MessageType::DOCK, actual->type());
  EXPECT_EQ(Uuid{19}, actual->as<TestSystemMessage>().getSystemDbId());

  EXPECT_TRUE(system20Queue->empty());
}

TEST(Unit_Server_Consumers_TriageMessageConsumer, ThrowsWhenSystemMessageReferencesUnknownSystem)
{
  // Empty map to not have any available system queue
  TriageMessageConsumer consumer(SystemQueueMap{});

  EXPECT_THAT(
    [&consumer]() {
      TestSystemMessage message(Uuid{19});
      consumer.onMessageReceived(message);
    },
    ThrowsMessage<::core::CoreException>("Failed to process message dock"));
}

} // namespace bsgo
