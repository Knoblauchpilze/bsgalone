
#include "LoginUseCase.hh"
#include "MockAccountRepository.hh"
#include "MockClientManager.hh"
#include "MockPlayerRepository.hh"
#include "PlayerLoginEvent.hh"
#include "TestGameEventPublisher.hh"
#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace test;
using namespace ::testing;

namespace bsgalone::server {

TEST(Unit_Bsgalone_Server_Domain_App_Usecases_LoginUseCase, ThrowsWhenAccountRepositoryIsNull)
{
  EXPECT_THROW(
    []() {
      auto mockPlayerRepo    = std::make_shared<MockPlayerRepository>();
      auto mockClientManager = std::make_shared<MockClientManager>();
      auto publisher         = std::make_shared<TestGameEventPublisher>();
      LoginUseCase(nullptr, mockPlayerRepo, mockClientManager, publisher);
    }(),
    std::invalid_argument);
}

TEST(Unit_Bsgalone_Server_Domain_App_Usecases_LoginUseCase, ThrowsWhenPlayerRepositoryIsNull)
{
  EXPECT_THROW(
    []() {
      auto mockAccountRepo   = std::make_shared<MockAccountRepository>();
      auto mockClientManager = std::make_shared<MockClientManager>();
      auto publisher         = std::make_shared<TestGameEventPublisher>();
      LoginUseCase(mockAccountRepo, nullptr, mockClientManager, publisher);
    }(),
    std::invalid_argument);
}

TEST(Unit_Bsgalone_Server_Domain_App_Usecases_LoginUseCase, ThrowsWhenClientManagerIsNull)
{
  EXPECT_THROW(
    []() {
      auto mockAccountRepo = std::make_shared<MockAccountRepository>();
      auto mockPlayerRepo  = std::make_shared<MockPlayerRepository>();
      auto publisher       = std::make_shared<TestGameEventPublisher>();
      LoginUseCase(mockAccountRepo, mockPlayerRepo, nullptr, publisher);
    }(),
    std::invalid_argument);
}

TEST(Unit_Bsgalone_Server_Domain_App_Usecases_LoginUseCase, ThrowsWhenEventPublisherIsNull)
{
  EXPECT_THROW(
    []() {
      auto mockAccountRepo   = std::make_shared<MockAccountRepository>();
      auto mockPlayerRepo    = std::make_shared<MockPlayerRepository>();
      auto mockClientManager = std::make_shared<MockClientManager>();
      LoginUseCase(mockAccountRepo, mockPlayerRepo, mockClientManager, nullptr);
    }(),
    std::invalid_argument);
}

TEST(Unit_Bsgalone_Server_Domain_App_Usecases_LoginUseCase,
     PublishesFailedLoginEventWhenAccountDoesNotExist)
{
  auto mockAccountRepo   = std::make_shared<StrictMock<MockAccountRepository>>();
  auto mockPlayerRepo    = std::make_shared<StrictMock<MockPlayerRepository>>();
  auto mockClientManager = std::make_shared<StrictMock<MockClientManager>>();
  auto publisher         = std::make_shared<TestGameEventPublisher>();
  LoginUseCase usecase(mockAccountRepo, mockPlayerRepo, mockClientManager, publisher);

  LoginData data{
    .username = "player",
    .password = "password",
    .role     = core::GameRole::PILOT,
    .clientId = net::ClientId{12},
  };

  EXPECT_CALL(*mockAccountRepo, findOneByName("player"))
    .Times(1)
    .WillOnce(Return(std::optional<Account>{}));

  usecase.performLogin(data);

  EXPECT_EQ(1u, publisher->queue().messages().size());
  const auto &event = publisher->queue().messages().at(0);
  EXPECT_EQ(GameEventType::PLAYER_LOGIN, event->type());
  EXPECT_EQ(data.clientId, event->as<PlayerLoginEvent>().getClientId());
  EXPECT_FALSE(event->as<PlayerLoginEvent>().successfulLogin());
}

TEST(Unit_Bsgalone_Server_Domain_App_Usecases_LoginUseCase,
     PublishesFailedLoginEventWhenCredentialsDoNotMatch)
{
  auto mockAccountRepo   = std::make_shared<StrictMock<MockAccountRepository>>();
  auto mockPlayerRepo    = std::make_shared<StrictMock<MockPlayerRepository>>();
  auto mockClientManager = std::make_shared<StrictMock<MockClientManager>>();
  auto publisher         = std::make_shared<TestGameEventPublisher>();
  LoginUseCase usecase(mockAccountRepo, mockPlayerRepo, mockClientManager, publisher);

  LoginData data{
    .username = "player",
    .password = "password",
    .role     = core::GameRole::PILOT,
    .clientId = net::ClientId{12},
  };

  Account account{
    .dbId     = core::Uuid{0},
    .username = "player",
    .password = "new-password",
  };
  EXPECT_CALL(*mockAccountRepo, findOneByName("player")).Times(1).WillOnce(Return(account));

  usecase.performLogin(data);

  EXPECT_EQ(1u, publisher->queue().messages().size());
  const auto &event = publisher->queue().messages().at(0);
  EXPECT_EQ(GameEventType::PLAYER_LOGIN, event->type());
  EXPECT_EQ(data.clientId, event->as<PlayerLoginEvent>().getClientId());
  EXPECT_FALSE(event->as<PlayerLoginEvent>().successfulLogin());
}

TEST(Unit_Bsgalone_Server_Domain_App_Usecases_LoginUseCase,
     PublishesSuccessfulLoginEventWhenLoginSucceed)
{
  auto mockAccountRepo   = std::make_shared<StrictMock<MockAccountRepository>>();
  auto mockPlayerRepo    = std::make_shared<StrictMock<MockPlayerRepository>>();
  auto mockClientManager = std::make_shared<NiceMock<MockClientManager>>();
  auto publisher         = std::make_shared<TestGameEventPublisher>();
  LoginUseCase usecase(mockAccountRepo, mockPlayerRepo, mockClientManager, publisher);

  LoginData data{
    .username = "player",
    .password = "password",
    .role     = core::GameRole::PILOT,
    .clientId = net::ClientId{12},
  };

  Account account{
    .dbId     = core::Uuid{145},
    .username = "player",
    .password = "password",
  };
  EXPECT_CALL(*mockAccountRepo, findOneByName("player")).Times(1).WillOnce(Return(account));

  Player player{
    .dbId    = core::Uuid{236},
    .account = account.dbId,
    .name    = "player",
    .faction = core::Faction::COLONIAL,
    .role    = core::GameRole::GUNNER,
  };
  EXPECT_CALL(*mockPlayerRepo, findOneByAccount(account.dbId)).Times(1).WillOnce(Return(player));

  Player expectedPlayer = player;
  expectedPlayer.role   = data.role;
  EXPECT_CALL(*mockPlayerRepo, save(expectedPlayer)).Times(1).WillOnce(Return(expectedPlayer));

  usecase.performLogin(data);

  EXPECT_EQ(1u, publisher->queue().messages().size());
  const auto &event = publisher->queue().messages().at(0);
  EXPECT_EQ(GameEventType::PLAYER_LOGIN, event->type());
  EXPECT_EQ(data.clientId, event->as<PlayerLoginEvent>().getClientId());
  EXPECT_TRUE(event->as<PlayerLoginEvent>().successfulLogin());
  EXPECT_EQ(player.dbId, event->as<PlayerLoginEvent>().tryGetPlayerDbId());
  EXPECT_EQ(data.role, event->as<PlayerLoginEvent>().tryGetRole());
}

TEST(Unit_Bsgalone_Server_Domain_App_Usecases_LoginUseCase, RegistersPlayerWhenLoginSucceeds)
{
  auto mockAccountRepo   = std::make_shared<MockAccountRepository>();
  auto mockPlayerRepo    = std::make_shared<MockPlayerRepository>();
  auto mockClientManager = std::make_shared<StrictMock<MockClientManager>>();
  auto publisher         = std::make_shared<TestGameEventPublisher>();
  LoginUseCase usecase(mockAccountRepo, mockPlayerRepo, mockClientManager, publisher);

  LoginData data{
    .username = "player",
    .password = "password",
    .role     = core::GameRole::PILOT,
    .clientId = net::ClientId{12},
  };

  Account account{
    .dbId     = core::Uuid{145},
    .username = "player",
    .password = "password",
  };
  EXPECT_CALL(*mockAccountRepo, findOneByName("player")).WillOnce(Return(account));

  Player player{
    .dbId    = core::Uuid{236},
    .account = account.dbId,
    .name    = "player",
    .faction = core::Faction::COLONIAL,
    .role    = core::GameRole::GUNNER,
  };
  EXPECT_CALL(*mockPlayerRepo, findOneByAccount(account.dbId)).WillOnce(Return(player));

  Player expectedPlayer = player;
  expectedPlayer.role   = data.role;
  EXPECT_CALL(*mockPlayerRepo, save(expectedPlayer)).WillOnce(Return(expectedPlayer));

  EXPECT_CALL(*mockClientManager, registerPlayer(net::ClientId{12}, core::Uuid{236}, core::Uuid{0}))
    .Times(1);

  usecase.performLogin(data);

  EXPECT_EQ(1u, publisher->queue().messages().size());
  const auto &event = publisher->queue().messages().at(0);
  EXPECT_EQ(GameEventType::PLAYER_LOGIN, event->type());
  EXPECT_EQ(data.clientId, event->as<PlayerLoginEvent>().getClientId());
  EXPECT_TRUE(event->as<PlayerLoginEvent>().successfulLogin());
  EXPECT_EQ(player.dbId, event->as<PlayerLoginEvent>().tryGetPlayerDbId());
  EXPECT_EQ(data.role, event->as<PlayerLoginEvent>().tryGetRole());
}

} // namespace bsgalone::server
