
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

TEST(Unit_Bsgalone_Server_Domain_App_Usecases_LogoutUseCase, TODO_GiveABetterName)
{
  auto mockClientManager = std::make_shared<StrictMock<MockClientManager>>();
  auto publisher         = std::make_shared<StrictMock<TestGameEventPublisher>>();
  LogoutUseCase usecase(mockClientManager, publisher);

  LogoutData data{.playerDbId = core::Uuid{17}};

  usecase.performLogout(data);

  // TODO: Assert things.
}

} // namespace bsgalone::server
