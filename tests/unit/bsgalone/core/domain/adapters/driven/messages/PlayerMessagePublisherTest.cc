
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

namespace {
bool doesSystemExist(const std::vector<System> &systems, const System &expected)
{
  const auto maybeSystem = std::find_if(systems.begin(),
                                        systems.end(),
                                        [&expected](const System &data) {
                                          return data.dbId == expected.dbId
                                                 && data.name == expected.name
                                                 && data.position == expected.position
                                                 && data.step == expected.step
                                                 && data.currentTick == expected.currentTick;
                                        });
  return maybeSystem != systems.end();
}
} // namespace

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
  System system2{.dbId        = Uuid{20},
                 .name        = "system2",
                 .position    = Eigen::Vector3f(-98.75f, -14.745f, 36.01f),
                 .currentTick = chrono::Tick::fromInt(2)};

  publisher.publishSystemList(Uuid{18}, {system1, system2});

  EXPECT_EQ(1u, queue->messages().size());
  EXPECT_EQ(MessageType::SYSTEM_LIST, queue->messages().at(0)->type());
  const auto &actual = queue->messages().at(0)->as<SystemListMessage>();
  EXPECT_EQ(Uuid{18}, actual.getPlayerDbId());
  // TODO: Should be a real system
  EXPECT_EQ(Uuid{0}, actual.getSystemDbId());
  const auto systems = actual.getSystemsData();

  EXPECT_TRUE(doesSystemExist(systems, system1))
    << "Expected to find system " << system1.name << " among " << systems.size();
  EXPECT_TRUE(doesSystemExist(systems, system2))
    << "Expected to find system " << system2.name << " among " << systems.size();
}

} // namespace bsgalone::core
