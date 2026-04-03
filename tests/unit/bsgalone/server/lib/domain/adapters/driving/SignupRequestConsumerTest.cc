
#include "SignupRequestConsumer.hh"
#include "SignupRequest.hh"
#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace ::testing;

namespace bsgalone::server {
namespace {
class MockUseCase : public core::ForExecutingSignup
{
  public:
  MockUseCase()           = default;
  ~MockUseCase() override = default;

  MOCK_METHOD(void, performSignup, (const core::SignupData &), (override));
};
} // namespace

TEST(Unit_Bsgalone_Core_Domain_Adapters_Driving_SignupRequestConsumer, ThrowsWhenUseCaseIsNull)
{
  EXPECT_THROW([]() { SignupRequestConsumer(nullptr); }(), std::invalid_argument);
}

TEST(Unit_Bsgalone_Core_Domain_Adapters_Driving_SignupRequestConsumer,
     ConsidersSignupRequestEventAsRelevant)
{
  SignupRequestConsumer consumer(std::make_unique<MockUseCase>());

  EXPECT_TRUE(consumer.isEventRelevant(core::MessageType::SIGNUP_REQUEST));
}

TEST(Unit_Bsgalone_Core_Domain_Adapters_Driving_SignupRequestConsumer,
     IgnoresSignupRequestWhenClientIdIsNotSet)
{
  auto usecase = std::make_unique<StrictMock<MockUseCase>>();
  EXPECT_CALL(*usecase, performSignup(_)).Times(0);

  SignupRequestConsumer consumer(std::move(usecase));

  core::SignupRequest request("username", "password", core::Faction::COLONIAL);
  consumer.onEventReceived(request);
}

TEST(Unit_Bsgalone_Core_Domain_Adapters_Driving_SignupRequestConsumer,
     DelegatesSignupToUseCaseWhenClientIdIsSet)
{
  auto usecase = std::make_unique<StrictMock<MockUseCase>>();
  core::SignupData captured{};
  EXPECT_CALL(*usecase, performSignup(_))
    .Times(1)
    .WillOnce(Invoke([&captured](const core::SignupData &data) { captured = data; }));

  core::SignupRequest request("username", "password", core::Faction::CYLON);
  request.setClientId(net::ClientId{12});

  SignupRequestConsumer consumer(std::move(usecase));
  consumer.onEventReceived(request);

  EXPECT_EQ("username", captured.username);
  EXPECT_EQ("password", captured.password);
  EXPECT_EQ(core::Faction::CYLON, captured.faction);
  EXPECT_EQ(net::ClientId{12}, captured.clientId);
}

} // namespace bsgalone::server
