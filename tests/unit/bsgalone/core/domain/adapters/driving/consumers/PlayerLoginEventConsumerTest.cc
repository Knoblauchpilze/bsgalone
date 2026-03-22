
#include "PlayerLoginEventConsumer.hh"
#include "PlayerLoginEvent.hh"
#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace ::testing;

namespace bsgalone::core {
namespace {
class MockPublisher : public ForPublishingLoginData
{
  public:
  MockPublisher()           = default;
  ~MockPublisher() override = default;

  MOCK_METHOD(void, publishLoginData, (const Uuid), (override));
};
} // namespace

TEST(Unit_Bsgalone_Core_Domain_Adapters_Driving_Consumers_PlayerLoginEventConsumer,
     ConsidersPlayerLoginReventRelevant)
{
  PlayerLoginEventConsumer consumer(std::make_unique<MockPublisher>());

  EXPECT_TRUE(consumer.isEventRelevant(GameEventType::PLAYER_LOGIN));
}

TEST(Unit_Bsgalone_Core_Domain_Adapters_Driving_Consumers_PlayerLoginEventConsumer,
     ForwardsSuccessfulLoginMessageToUseCase)
{
  PlayerLoginEvent event;
  event.setPlayerDbId(Uuid{18});
  event.setRole(GameRole::GUNNER);
  event.setSystemDbId(Uuid{19});

  auto mockPublisher = std::make_unique<StrictMock<MockPublisher>>();
  EXPECT_CALL(*mockPublisher, publishLoginData(Uuid{18})).Times(1);

  PlayerLoginEventConsumer consumer(std::move(mockPublisher));
  consumer.onEventReceived(event);
}

TEST(Unit_Bsgalone_Core_Domain_Adapters_Driving_Consumers_PlayerLoginEventConsumer,
     DoesNotFailedLoginMessage)
{
  PlayerLoginEventConsumer consumer(std::make_unique<StrictMock<MockPublisher>>());

  PlayerLoginEvent event;
  consumer.onEventReceived(event);
}

} // namespace bsgalone::core
