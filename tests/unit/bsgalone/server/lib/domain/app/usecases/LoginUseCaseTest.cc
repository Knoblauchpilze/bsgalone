
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
namespace {
class Unit_Bsgalone_Server_Domain_App_Usecases_LoginUseCase : public Test
{
  protected:
  void SetUp() override
  {
    mockAccountRepo   = std::make_shared<StrictMock<MockAccountRepository>>();
    mockPlayerRepo    = std::make_shared<StrictMock<MockPlayerRepository>>();
    mockClientManager = std::make_shared<StrictMock<MockClientManager>>();
    publisher         = std::make_shared<TestGameEventPublisher>();

    usecase = std::make_unique<LoginUseCase>(mockAccountRepo,
                                             mockPlayerRepo,
                                             mockClientManager,
                                             publisher);
  }

  void TearDown() override
  {
    // Intentionally empty
  }

  void executeTestCase()
  {
    LoginData data{
      .username = "player",
      .password = "password",
      .role     = core::GameRole::PILOT,
      .clientId = net::ClientId{12},
    };

    usecase->performLogin(data);
  }

  std::shared_ptr<MockAccountRepository> mockAccountRepo{};
  std::shared_ptr<MockPlayerRepository> mockPlayerRepo{};
  std::shared_ptr<MockClientManager> mockClientManager{};
  std::shared_ptr<TestGameEventPublisher> publisher{};

  std::unique_ptr<LoginUseCase> usecase{};
};
} // namespace

TEST_F(Unit_Bsgalone_Server_Domain_App_Usecases_LoginUseCase, ThrowsWhenAccountRepositoryIsNull)
{
  EXPECT_THROW([this]() { LoginUseCase(nullptr, mockPlayerRepo, mockClientManager, publisher); }(),
               std::invalid_argument);
}

TEST_F(Unit_Bsgalone_Server_Domain_App_Usecases_LoginUseCase, ThrowsWhenPlayerRepositoryIsNull)
{
  EXPECT_THROW([this]() { LoginUseCase(mockAccountRepo, nullptr, mockClientManager, publisher); }(),
               std::invalid_argument);
}

TEST_F(Unit_Bsgalone_Server_Domain_App_Usecases_LoginUseCase, ThrowsWhenClientManagerIsNull)
{
  EXPECT_THROW([this]() { LoginUseCase(mockAccountRepo, mockPlayerRepo, nullptr, publisher); }(),
               std::invalid_argument);
}

TEST_F(Unit_Bsgalone_Server_Domain_App_Usecases_LoginUseCase, ThrowsWhenEventPublisherIsNull)
{
  EXPECT_THROW(
    [this]() { LoginUseCase(mockAccountRepo, mockPlayerRepo, mockClientManager, nullptr); }(),
    std::invalid_argument);
}

TEST_F(Unit_Bsgalone_Server_Domain_App_Usecases_LoginUseCase,
       PublishesFailedLoginEventWhenAccountDoesNotExist)
{
  EXPECT_CALL(*mockAccountRepo, findOneByName("player"))
    .Times(1)
    .WillOnce(Return(std::optional<Account>{}));

  this->executeTestCase();

  EXPECT_EQ(1u, publisher->queue().messages().size());
  const auto &event = publisher->queue().messages().at(0);
  EXPECT_EQ(GameEventType::PLAYER_LOGIN, event->type());
  EXPECT_EQ(net::ClientId{12}, event->as<PlayerLoginEvent>().getClientId());
  EXPECT_FALSE(event->as<PlayerLoginEvent>().successfulLogin());
}

TEST_F(Unit_Bsgalone_Server_Domain_App_Usecases_LoginUseCase,
       PublishesFailedLoginEventWhenCredentialsDoNotMatch)
{
  Account account{
    .dbId     = core::Uuid{},
    .username = "player",
    .password = "new-password",
  };
  EXPECT_CALL(*mockAccountRepo, findOneByName("player")).Times(1).WillOnce(Return(account));

  this->executeTestCase();

  EXPECT_EQ(1u, publisher->queue().messages().size());
  const auto &event = publisher->queue().messages().at(0);
  EXPECT_EQ(GameEventType::PLAYER_LOGIN, event->type());
  EXPECT_EQ(net::ClientId{12}, event->as<PlayerLoginEvent>().getClientId());
  EXPECT_FALSE(event->as<PlayerLoginEvent>().successfulLogin());
}

TEST_F(Unit_Bsgalone_Server_Domain_App_Usecases_LoginUseCase,
       PublishesSuccessfulLoginEventWhenLoginSucceeds)
{
  Account account{
    .dbId     = core::Uuid{},
    .username = "player",
    .password = "password",
  };
  EXPECT_CALL(*mockAccountRepo, findOneByName("player")).Times(1).WillOnce(Return(account));

  Player player{
    .dbId    = core::Uuid{},
    .account = account.dbId,
    .name    = "player",
    .faction = core::Faction::COLONIAL,
    .role    = core::GameRole::GUNNER,
  };
  EXPECT_CALL(*mockPlayerRepo, findOneByAccount(account.dbId)).Times(1).WillOnce(Return(player));

  Player expectedPlayer = player;
  expectedPlayer.role   = core::GameRole::PILOT;
  EXPECT_CALL(*mockPlayerRepo, save(expectedPlayer)).Times(1);

  EXPECT_CALL(*mockClientManager,
              registerPlayer(net::ClientId{12},
                             player.dbId,
                             core::Uuid::fromDbId("7b83bcfe-2785-40e9-894a-04f21a6346ac")))
    .Times(1);

  this->executeTestCase();

  EXPECT_EQ(1u, publisher->queue().messages().size());
  const auto &event = publisher->queue().messages().at(0);
  EXPECT_EQ(GameEventType::PLAYER_LOGIN, event->type());
  const auto &actual = event->as<PlayerLoginEvent>();
  EXPECT_EQ(net::ClientId{12}, actual.getClientId());
  EXPECT_TRUE(actual.successfulLogin());
  EXPECT_EQ(player.dbId, actual.tryGetPlayerDbId());
  EXPECT_EQ(player.faction, actual.tryGetFaction());
  EXPECT_EQ(core::GameRole::PILOT, actual.tryGetRole());
}

TEST_F(Unit_Bsgalone_Server_Domain_App_Usecases_LoginUseCase, RegistersPlayerWhenLoginSucceeds)
{
  Account account{
    .dbId     = core::Uuid{},
    .username = "player",
    .password = "password",
  };
  EXPECT_CALL(*mockAccountRepo, findOneByName("player")).WillOnce(Return(account));

  Player player{
    .dbId    = core::Uuid{},
    .account = account.dbId,
    .name    = "player",
    .faction = core::Faction::COLONIAL,
    .role    = core::GameRole::GUNNER,
  };
  EXPECT_CALL(*mockPlayerRepo, findOneByAccount(account.dbId)).WillOnce(Return(player));

  Player expectedPlayer = player;
  expectedPlayer.role   = core::GameRole::PILOT;
  EXPECT_CALL(*mockPlayerRepo, save(expectedPlayer)).Times(1);

  EXPECT_CALL(*mockClientManager,
              registerPlayer(net::ClientId{12},
                             player.dbId,
                             core::Uuid::fromDbId("7b83bcfe-2785-40e9-894a-04f21a6346ac")))
    .Times(1);

  this->executeTestCase();
}

} // namespace bsgalone::server
