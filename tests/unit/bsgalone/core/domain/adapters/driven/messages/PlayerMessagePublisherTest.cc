
#include "PlayerMessagePublisher.hh"
#include "SystemListMessage.hh"
#include "TestMessageQueue.hh"
#include <gtest/gtest.h>

using namespace test;
using namespace ::testing;

namespace bsgalone::core {

TEST(Unit_Bsgalone_Core_Domain_Adapters_Driven_Messages_PlayerMessagePublisher,
     ThrowsWhenEventBusIsNull)
{
  EXPECT_THROW([]() { PlayerMessagePublisher(nullptr); }(), std::invalid_argument);
}

TEST(Unit_Bsgalone_Core_Domain_Adapters_Driven_Messages_PlayerMessagePublisher,
     DoesNotPublishWhenSystemListIsEmpty)
{
  auto queue = std::make_shared<TestMessageQueue>();
  PlayerMessagePublisher publisher(queue);

  publisher.publishSystemList(Uuid{18}, {});

  EXPECT_TRUE(queue->empty());
}

TEST(Unit_Bsgalone_Core_Domain_Adapters_Driven_Messages_PlayerMessagePublisher,
     PublishesSystemListMessage)
{
  auto queue = std::make_shared<TestMessageQueue>();
  PlayerMessagePublisher publisher(queue);

  System system1{
    .dbId     = Uuid{19},
    .name     = "random-name",
    .position = Eigen::Vector3f(1.0f, -2.0f, 3.0f),
  };
  System system2{
    .dbId     = Uuid{20},
    .name     = "system2",
    .position = Eigen::Vector3f(-98.75f, -14.745f, 36.01f),
  };

  publisher.publishSystemList(Uuid{18}, {system1, system2});

  EXPECT_EQ(1u, queue->messages().size());
  EXPECT_EQ(MessageType::SYSTEM_LIST, queue->messages().at(0)->type());
  const auto &actual = queue->messages().at(0)->as<SystemListMessage>();
  EXPECT_EQ(Uuid{18}, actual.getPlayerDbId());
  // TODO: Should be a real system
  EXPECT_EQ(Uuid{0}, actual.getSystemDbId());
  const auto systems = actual.getSystemsData();

  const auto maybeSystem1 = std::find_if(systems.begin(),
                                         systems.end(),
                                         [&system1](const SystemData &data) {
                                           return data.dbId == system1.dbId
                                                  && data.name == system1.name
                                                  && data.position == system1.position;
                                         });
  EXPECT_NE(maybeSystem1, systems.end())
    << "Expected to find system " << system1.name << " among " << systems.size();
  const auto maybeSystem2 = std::find_if(systems.begin(),
                                         systems.end(),
                                         [&system2](const SystemData &data) {
                                           return data.dbId == system2.dbId
                                                  && data.name == system2.name
                                                  && data.position == system2.position;
                                         });
  EXPECT_NE(maybeSystem2, systems.end())
    << "Expected to find system " << system2.name << " among " << systems.size();
}

} // namespace bsgalone::core
