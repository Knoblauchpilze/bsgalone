
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
  auto publisher       = std::make_shared<StrictMock<TestGameEventPublisher>>();
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
  auto publisher       = std::make_shared<StrictMock<TestGameEventPublisher>>();
  SignupUseCase usecase(mockAccountRepo, mockPlayerRepo, publisher);

  SignupData data{
    .username = "player",
    .password = "password",
    .faction  = core::Faction::COLONIAL,
    .clientId = net::ClientId{12},
  };

  EXPECT_CALL(*mockAccountRepo, save(_))
    .Times(1)
    .WillOnce(Return(Account{
      .dbId     = core::Uuid{2},
      .username = "player",
      .password = "password",
    }));
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
  auto publisher       = std::make_shared<StrictMock<TestGameEventPublisher>>();
  SignupUseCase usecase(mockAccountRepo, mockPlayerRepo, publisher);

  SignupData data{
    .username = "player",
    .password = "password",
    .faction  = core::Faction::COLONIAL,
    .clientId = net::ClientId{12},
  };

  EXPECT_CALL(*mockAccountRepo, save(_))
    .Times(1)
    .WillOnce(Return(Account{
      .dbId     = core::Uuid{2},
      .username = data.username,
      .password = data.password,
    }));
  EXPECT_CALL(*mockPlayerRepo, save(_))
    .Times(1)
    .WillOnce(Return(Player{
      .dbId    = core::Uuid{17},
      .account = core::Uuid{2},
      .name    = data.username,
      .faction = data.faction,
      .role    = core::GameRole::PILOT,
    }));

  usecase.performSignup(data);

  EXPECT_EQ(1u, publisher->queue().messages().size());
  const auto &event = publisher->queue().messages().at(0);
  EXPECT_EQ(GameEventType::PLAYER_SIGNUP, event->type());
  const auto &actual = event->as<PlayerSignupEvent>();
  EXPECT_EQ(data.clientId, actual.getClientId());
  EXPECT_TRUE(actual.successfulSignup());
  EXPECT_EQ(core::Uuid{17}, actual.tryGetPlayerDbId().value());
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
    return account;
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

  EXPECT_CALL(*mockAccountRepo, save(_))
    .Times(1)
    .WillOnce(Return(Account{
      .dbId     = core::Uuid{2},
      .username = data.username,
      .password = data.password,
    }));

  Player captured{};
  EXPECT_CALL(*mockPlayerRepo, save(_)).Times(1).WillOnce(Invoke([&captured](Player player) {
    captured = player;
    return player;
  }));

  usecase.performSignup(data);

  EXPECT_EQ(core::Uuid{2}, captured.account);
  EXPECT_EQ(data.username, captured.name);
  EXPECT_EQ(data.faction, captured.faction);
  EXPECT_EQ(core::GameRole::PILOT, captured.role);
}

} // namespace bsgalone::server
