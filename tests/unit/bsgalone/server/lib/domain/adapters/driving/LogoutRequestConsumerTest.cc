
#include "LogoutRequestConsumer.hh"
#include "LogoutRequest.hh"
#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace ::testing;

namespace bsgalone::server {
namespace {
class MockUseCase : public ForExecutingLogout
{
  public:
  MockUseCase()           = default;
  ~MockUseCase() override = default;

  MOCK_METHOD(void, performLogout, (const LogoutData &), (override));
};
} // namespace

TEST(Unit_Bsgalone_Server_Domain_Adapters_Driving_LogoutRequestConsumer, ThrowsWhenUseCaseIsNull)
{
  EXPECT_THROW([]() { LogoutRequestConsumer(nullptr); }(), std::invalid_argument);
}

TEST(Unit_Bsgalone_Server_Domain_Adapters_Driving_LogoutRequestConsumer,
     ConsidersLogoutRequestEventAsRelevant)
{
  LogoutRequestConsumer consumer(std::make_unique<MockUseCase>());

  EXPECT_TRUE(consumer.isEventRelevant(core::MessageType::LOGOUT_REQUEST));
}

TEST(Unit_Bsgalone_Server_Domain_Adapters_Driving_LogoutRequestConsumer, DelegatesLogoutToUseCase)
{
  auto usecase = std::make_unique<StrictMock<MockUseCase>>();
  LogoutData captured{};
  EXPECT_CALL(*usecase, performLogout(_))
    .Times(1)
    .WillOnce(Invoke([&captured](const LogoutData &data) { captured = data; }));

  core::LogoutRequest request(core::Uuid{371});

  LogoutRequestConsumer consumer(std::move(usecase));
  consumer.onEventReceived(request);

  EXPECT_EQ(core::Uuid{371}, captured.playerDbId);
}

} // namespace bsgalone::server
