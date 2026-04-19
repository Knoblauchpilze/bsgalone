
#include "SignupUseCase.hh"
#include "MockAccountRepository.hh"
#include "MockPlayerRepository.hh"
#include "PlayerSignupEvent.hh"
#include "TestGameEventPublisher.hh"
#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace test;
using namespace ::testing;

namespace bsgalone::server {

TEST(Unit_Bsgalone_Server_Domain_App_Usecases_SignupUseCase, ThrowsWhenAccountRepositoryIsNull)
{
  EXPECT_THROW(
    []() {
      auto mockPlayerRepo = std::make_shared<MockPlayerRepository>();
      auto publisher      = std::make_shared<TestGameEventPublisher>();
      SignupUseCase(nullptr, mockPlayerRepo, publisher);
    }(),
    std::invalid_argument);
}

TEST(Unit_Bsgalone_Server_Domain_App_Usecases_SignupUseCase, ThrowsWhenPlayerRepositoryIsNull)
{
  EXPECT_THROW(
    []() {
      auto mockAccountRepo = std::make_shared<MockAccountRepository>();
      auto publisher       = std::make_shared<TestGameEventPublisher>();
      SignupUseCase(mockAccountRepo, nullptr, publisher);
    }(),
    std::invalid_argument);
}

TEST(Unit_Bsgalone_Server_Domain_App_Usecases_SignupUseCase, ThrowsWhenEventPublisherIsNull)
{
  EXPECT_THROW(
    []() {
      auto mockAccountRepo = std::make_shared<MockAccountRepository>();
      auto mockPlayerRepo  = std::make_shared<MockPlayerRepository>();
      SignupUseCase(mockAccountRepo, mockPlayerRepo, nullptr);
    }(),
    std::invalid_argument);
}

TEST(Unit_Bsgalone_Server_Domain_App_Usecases_SignupUseCase,
     PublishesFailedSignupEventWhenSavingAccountFails)
{
  auto mockAccountRepo = std::make_shared<StrictMock<MockAccountRepository>>();
  auto mockPlayerRepo  = std::make_shared<StrictMock<MockPlayerRepository>>();
  auto publisher       = std::make_shared<TestGameEventPublisher>();
  SignupUseCase usecase(mockAccountRepo, mockPlayerRepo, publisher);

  SignupData data{
    .username = "player",
    .password = "password",
    .faction  = core::Faction::COLONIAL,
    .clientId = net::ClientId{12},
  };

  EXPECT_CALL(*mockAccountRepo, save(_))
    .Times(1)
    .WillOnce(Throw(std::runtime_error("Stubbed error")));

  usecase.performSignup(data);

  EXPECT_EQ(1u, publisher->queue().messages().size());
  const auto &event = publisher->queue().messages().at(0);
  EXPECT_EQ(GameEventType::PLAYER_SIGNUP, event->type());
  const auto &actual = event->as<PlayerSignupEvent>();
  EXPECT_EQ(data.clientId, actual.getClientId());
  EXPECT_FALSE(actual.successfulSignup());
}

TEST(Unit_Bsgalone_Server_Domain_App_Usecases_SignupUseCase,
     PublishesFailedSignupEventWhenSavingPlayerFails)
{
  auto mockAccountRepo = std::make_shared<StrictMock<MockAccountRepository>>();
  auto mockPlayerRepo  = std::make_shared<StrictMock<MockPlayerRepository>>();
  auto publisher       = std::make_shared<TestGameEventPublisher>();
  SignupUseCase usecase(mockAccountRepo, mockPlayerRepo, publisher);

  SignupData data{
    .username = "player",
    .password = "password",
    .faction  = core::Faction::COLONIAL,
    .clientId = net::ClientId{12},
  };

  EXPECT_CALL(*mockAccountRepo, save(_)).Times(1);
  EXPECT_CALL(*mockPlayerRepo, save(_)).Times(1).WillOnce(Throw(std::runtime_error("Stubbed error")));

  usecase.performSignup(data);

  EXPECT_EQ(1u, publisher->queue().messages().size());
  const auto &event = publisher->queue().messages().at(0);
  EXPECT_EQ(GameEventType::PLAYER_SIGNUP, event->type());
  const auto &actual = event->as<PlayerSignupEvent>();
  EXPECT_EQ(data.clientId, actual.getClientId());
  EXPECT_FALSE(actual.successfulSignup());
}

TEST(Unit_Bsgalone_Server_Domain_App_Usecases_SignupUseCase,
     PublishesSuccessfulSignupEventWhenSignupSucceed)
{
  auto mockAccountRepo = std::make_shared<StrictMock<MockAccountRepository>>();
  auto mockPlayerRepo  = std::make_shared<StrictMock<MockPlayerRepository>>();
  auto publisher       = std::make_shared<TestGameEventPublisher>();
  SignupUseCase usecase(mockAccountRepo, mockPlayerRepo, publisher);

  SignupData data{
    .username = "player",
    .password = "password",
    .faction  = core::Faction::COLONIAL,
    .clientId = net::ClientId{12},
  };

  const core::Uuid playerDbId;

  EXPECT_CALL(*mockAccountRepo, save(_)).Times(1);
  Player captured;
  EXPECT_CALL(*mockPlayerRepo, save(_)).Times(1).WillOnce(Invoke([&captured](Player player) {
    captured = player;
  }));

  usecase.performSignup(data);

  EXPECT_EQ(1u, publisher->queue().messages().size());
  const auto &event = publisher->queue().messages().at(0);
  EXPECT_EQ(GameEventType::PLAYER_SIGNUP, event->type());
  const auto &actual = event->as<PlayerSignupEvent>();
  EXPECT_EQ(data.clientId, actual.getClientId());
  EXPECT_TRUE(actual.successfulSignup());
  EXPECT_EQ(captured.dbId, actual.tryGetPlayerDbId().value());
  EXPECT_EQ(data.faction, actual.tryGetFaction());
}

TEST(Unit_Bsgalone_Server_Domain_App_Usecases_SignupUseCase, DelegatesAccountCreationToAdapter)
{
  auto mockAccountRepo = std::make_shared<StrictMock<MockAccountRepository>>();
  SignupUseCase usecase(mockAccountRepo,
                        std::make_shared<MockPlayerRepository>(),
                        std::make_shared<TestGameEventPublisher>());

  SignupData data{
    .username = "player",
    .password = "password",
    .faction  = core::Faction::COLONIAL,
    .clientId = net::ClientId{12},
  };

  Account captured{};
  EXPECT_CALL(*mockAccountRepo, save(_)).Times(1).WillOnce(Invoke([&captured](Account account) {
    captured = account;
  }));

  usecase.performSignup(data);

  EXPECT_EQ(data.username, captured.username);
  EXPECT_EQ(data.password, captured.password);
}

TEST(Unit_Bsgalone_Server_Domain_App_Usecases_SignupUseCase, DelegatesPlayerCreationToAdapter)
{
  auto mockAccountRepo = std::make_shared<StrictMock<MockAccountRepository>>();
  auto mockPlayerRepo  = std::make_shared<StrictMock<MockPlayerRepository>>();
  SignupUseCase usecase(mockAccountRepo, mockPlayerRepo, std::make_shared<TestGameEventPublisher>());

  SignupData data{
    .username = "player",
    .password = "password",
    .faction  = core::Faction::COLONIAL,
    .clientId = net::ClientId{12},
  };

  const core::Uuid accountDbId;

  Account capturedAccount{};
  EXPECT_CALL(*mockAccountRepo, save(_))
    .Times(1)
    .WillOnce(Invoke([&capturedAccount](Account account) { capturedAccount = account; }));

  Player capturedPlayer{};
  EXPECT_CALL(*mockPlayerRepo, save(_)).Times(1).WillOnce(Invoke([&capturedPlayer](Player player) {
    capturedPlayer = player;
  }));

  usecase.performSignup(data);

  EXPECT_EQ(capturedAccount.dbId, capturedPlayer.account.value());
  EXPECT_EQ(data.username, capturedPlayer.name);
  EXPECT_EQ(data.faction, capturedPlayer.faction);
  EXPECT_EQ(core::GameRole::PILOT, capturedPlayer.role);
}

} // namespace bsgalone::server
