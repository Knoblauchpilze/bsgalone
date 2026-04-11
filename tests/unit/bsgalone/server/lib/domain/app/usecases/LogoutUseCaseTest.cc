
#include "LogoutUseCase.hh"
#include "MockClientManager.hh"
#include "TestGameEventPublisher.hh"
#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace test;
using namespace ::testing;

namespace bsgalone::server {

TEST(Unit_Bsgalone_Server_Domain_App_Usecases_LogoutUseCase, ThrowsWhenClientManagerIsNull)
{
  EXPECT_THROW(
    []() {
      auto publisher = std::make_shared<TestGameEventPublisher>();
      LogoutUseCase(nullptr, publisher);
    }(),
    std::invalid_argument);
}

TEST(Unit_Bsgalone_Server_Domain_App_Usecases_LogoutUseCase, ThrowsWhenEventPublisherIsNull)
{
  EXPECT_THROW(
    []() {
      auto mockClientManager = std::make_shared<MockClientManager>();
      LogoutUseCase(mockClientManager, nullptr);
    }(),
    std::invalid_argument);
}

TEST(Unit_Bsgalone_Server_Domain_App_Usecases_LogoutUseCase, RemovesPlayerFromClient)
{
  auto mockClientManager = std::make_shared<StrictMock<MockClientManager>>();
  EXPECT_CALL(*mockClientManager, removePlayer(core::Uuid{17})).Times(1);

  auto publisher = std::make_shared<StrictMock<TestGameEventPublisher>>();
  LogoutUseCase usecase(mockClientManager, publisher);

  LogoutData data{.playerDbId = core::Uuid{17}};

  usecase.performLogout(data);
}

} // namespace bsgalone::server
