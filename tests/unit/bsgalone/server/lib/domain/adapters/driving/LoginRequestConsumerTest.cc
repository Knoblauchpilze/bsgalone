
#include "LoginRequestConsumer.hh"
#include "LoginRequest.hh"
#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace ::testing;

namespace bsgalone::server {
namespace {
class MockUseCase : public ForExecutingLogin
{
  public:
  MockUseCase()           = default;
  ~MockUseCase() override = default;

  MOCK_METHOD(void, performLogin, (const LoginData &), (override));
};
} // namespace

TEST(Unit_Bsgalone_Server_Domain_Adapters_Driving_LoginRequestConsumer, ThrowsWhenUseCaseIsNull)
{
  EXPECT_THROW([]() { LoginRequestConsumer(nullptr); }(), std::invalid_argument);
}

TEST(Unit_Bsgalone_Server_Domain_Adapters_Driving_LoginRequestConsumer,
     ConsidersLoginRequestEventAsRelevant)
{
  LoginRequestConsumer consumer(std::make_unique<MockUseCase>());

  EXPECT_TRUE(consumer.isEventRelevant(core::MessageType::LOGIN_REQUEST));
}

TEST(Unit_Bsgalone_Server_Domain_Adapters_Driving_LoginRequestConsumer,
     IgnoresLoginRequestWhenClientIdIsNotSet)
{
  auto usecase = std::make_unique<StrictMock<MockUseCase>>();
  EXPECT_CALL(*usecase, performLogin(_)).Times(0);

  LoginRequestConsumer consumer(std::move(usecase));

  core::LoginRequest request("username", "password", core::GameRole::PILOT);
  consumer.onEventReceived(request);
}

TEST(Unit_Bsgalone_Server_Domain_Adapters_Driving_LoginRequestConsumer,
     DelegatesLoginToUseCaseWhenClientIdIsSet)
{
  auto usecase = std::make_unique<StrictMock<MockUseCase>>();
  LoginData captured{};
  EXPECT_CALL(*usecase, performLogin(_)).Times(1).WillOnce(Invoke([&captured](const LoginData &data) {
    captured = data;
  }));

  core::LoginRequest request("username", "password", core::GameRole::PILOT);
  request.setClientId(net::ClientId{12});

  LoginRequestConsumer consumer(std::move(usecase));
  consumer.onEventReceived(request);

  EXPECT_EQ("username", captured.username);
  EXPECT_EQ("password", captured.password);
  EXPECT_EQ(core::GameRole::PILOT, captured.role);
  EXPECT_EQ(net::ClientId{12}, captured.clientId);
}

} // namespace bsgalone::server
