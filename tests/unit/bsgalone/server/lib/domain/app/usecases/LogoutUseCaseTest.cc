
#include "LogoutUseCase.hh"
#include "ClientManager.hh"
#include "MockClientManager.hh"
#include "PlayerLogoutEvent.hh"
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

  auto publisher = std::make_shared<TestGameEventPublisher>();
  LogoutUseCase usecase(mockClientManager, publisher);

  LogoutData data{.playerDbId = core::Uuid{17}};

  usecase.performLogout(data);
}

TEST(Unit_Bsgalone_Server_Domain_App_Usecases_LogoutUseCase,
     PublishesPlayerLogoutEventWhenPlayerRemoved)
{
  auto mockClientManager = std::make_shared<StrictMock<MockClientManager>>();
  EXPECT_CALL(*mockClientManager, removePlayer(core::Uuid{17})).Times(1);

  auto publisher = std::make_shared<TestGameEventPublisher>();
  LogoutUseCase usecase(mockClientManager, publisher);

  LogoutData data{.playerDbId = core::Uuid{17}};

  usecase.performLogout(data);

  EXPECT_EQ(1u, publisher->queue().messages().size());
  const auto &event = publisher->queue().messages().at(0);
  EXPECT_EQ(GameEventType::PLAYER_LOGOUT, event->type());
  const auto &actual = event->as<PlayerLogoutEvent>();
  EXPECT_EQ(data.playerDbId, actual.getPlayerDbId());
}

TEST(Unit_Bsgalone_Server_Domain_App_Usecases_LogoutUseCase,
     DoesNotPublishPlayerLogoutEventWhenPlayerIsNotRegistered)
{
  auto clientManager = std::make_shared<ClientManager>();
  auto publisher     = std::make_shared<TestGameEventPublisher>();
  LogoutUseCase usecase(clientManager, publisher);

  LogoutData data{.playerDbId = core::Uuid{17}};

  usecase.performLogout(data);

  EXPECT_TRUE(publisher->queue().messages().empty());
}

} // namespace bsgalone::server
