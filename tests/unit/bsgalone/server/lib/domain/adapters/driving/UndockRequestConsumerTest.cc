
#include "UndockRequestConsumer.hh"
#include "UndockRequest.hh"
#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace ::testing;

namespace bsgalone::server {
namespace {
class MockUseCase : public ForExecutingUndock
{
  public:
  MockUseCase()           = default;
  ~MockUseCase() override = default;

  MOCK_METHOD(void, performUndock, (const UndockData &), (override));
};
} // namespace

TEST(Unit_Bsgalone_Server_Domain_Adapters_Driving_UndockRequestConsumer, ThrowsWhenUseCaseIsNull)
{
  EXPECT_THROW([]() { UndockRequestConsumer(nullptr); }(), std::invalid_argument);
}

TEST(Unit_Bsgalone_Server_Domain_Adapters_Driving_UndockRequestConsumer,
     ConsidersUNdockRequestEventAsRelevant)
{
  UndockRequestConsumer consumer(std::make_unique<MockUseCase>());

  EXPECT_TRUE(consumer.isEventRelevant(core::MessageType::UNDOCK_REQUEST));
}

TEST(Unit_Bsgalone_Server_Domain_Adapters_Driving_UndockRequestConsumer, DelegatesUndockToUseCase)
{
  auto usecase = std::make_unique<StrictMock<MockUseCase>>();
  UndockData captured{};
  EXPECT_CALL(*usecase, performUndock(_))
    .Times(1)
    .WillOnce(Invoke([&captured](const UndockData &data) { captured = data; }));

  const core::Uuid playerDbId;
  core::UndockRequest request(playerDbId);

  UndockRequestConsumer consumer(std::move(usecase));
  consumer.onEventReceived(request);

  EXPECT_EQ(playerDbId, captured.playerDbId);
}

} // namespace bsgalone::server
