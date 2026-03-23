
#include "LoginUseCase.hh"
#include "PlayerLoginEvent.hh"
#include "TestEventPublisher.hh"
#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace test;
using namespace ::testing;

namespace bsgalone::core {
namespace {
class MockAccountRepo : public ForManagingAccount
{
  public:
  MockAccountRepo()           = default;
  ~MockAccountRepo() override = default;

  MOCK_METHOD(std::optional<Account>, findOneByName, (const std::string &), (const, override));
};

class MockPlayerRepo : public ForManagingPlayer
{
  public:
  MockPlayerRepo()           = default;
  ~MockPlayerRepo() override = default;

  MOCK_METHOD(Player, findOneById, (const Uuid), (const, override));
  MOCK_METHOD(Player, findOneByAccount, (const Uuid), (const, override));
  MOCK_METHOD(Player, save, (Player), (override));
};

} // namespace

TEST(Unit_Bsgalone_Core_Domain_App_Usecases_LoginUseCase, ThrowsWhenAccountRepositoryIsNull)
{
  EXPECT_THROW(
    []() {
      auto mockPlayerRepo = std::make_shared<MockPlayerRepo>();
      auto publisher      = std::make_shared<TestEventPublisher>();
      LoginUseCase(nullptr, mockPlayerRepo, publisher);
    }(),
    std::invalid_argument);
}

TEST(Unit_Bsgalone_Core_Domain_App_Usecases_LoginUseCase, ThrowsWhenPlayerRepositoryIsNull)
{
  EXPECT_THROW(
    []() {
      auto mockAccountRepo = std::make_shared<MockAccountRepo>();
      auto publisher       = std::make_shared<TestEventPublisher>();
      LoginUseCase(mockAccountRepo, nullptr, publisher);
    }(),
    std::invalid_argument);
}

TEST(Unit_Bsgalone_Core_Domain_App_Usecases_LoginUseCase, ThrowsWhenEventPublisherIsNull)
{
  EXPECT_THROW(
    []() {
      auto mockAccountRepo = std::make_shared<MockAccountRepo>();
      auto mockPlayerRepo  = std::make_shared<MockPlayerRepo>();
      LoginUseCase(mockAccountRepo, mockPlayerRepo, nullptr);
    }(),
    std::invalid_argument);
}

TEST(Unit_Bsgalone_Core_Domain_App_Usecases_LoginUseCase,
     PublishesFailedLoginEventWhenAccountDoesNotExist)
{
  auto mockAccountRepo = std::make_shared<StrictMock<MockAccountRepo>>();
  auto mockPlayerRepo  = std::make_shared<StrictMock<MockPlayerRepo>>();
  auto publisher       = std::make_shared<StrictMock<TestEventPublisher>>();
  LoginUseCase usecase(mockAccountRepo, mockPlayerRepo, publisher);

  LoginData data{
    .username = "player",
    .password = "password",
    .role     = GameRole::PILOT,
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

TEST(Unit_Bsgalone_Core_Domain_App_Usecases_LoginUseCase,
     PublishesFailedLoginEventWhenCredentialsDoNotMatch)
{
  auto mockAccountRepo = std::make_shared<StrictMock<MockAccountRepo>>();
  auto mockPlayerRepo  = std::make_shared<StrictMock<MockPlayerRepo>>();
  auto publisher       = std::make_shared<StrictMock<TestEventPublisher>>();
  LoginUseCase usecase(mockAccountRepo, mockPlayerRepo, publisher);

  LoginData data{
    .username = "player",
    .password = "password",
    .role     = GameRole::PILOT,
    .clientId = net::ClientId{12},
  };

  Account account{
    .dbId     = Uuid{0},
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

TEST(Unit_Bsgalone_Core_Domain_App_Usecases_LoginUseCase,
     PublishesSuccessfulLoginEventWhenLoginSucceed)
{
  auto mockAccountRepo = std::make_shared<StrictMock<MockAccountRepo>>();
  auto mockPlayerRepo  = std::make_shared<StrictMock<MockPlayerRepo>>();
  auto publisher       = std::make_shared<StrictMock<TestEventPublisher>>();
  LoginUseCase usecase(mockAccountRepo, mockPlayerRepo, publisher);

  LoginData data{
    .username = "player",
    .password = "password",
    .role     = GameRole::PILOT,
    .clientId = net::ClientId{12},
  };

  Account account{
    .dbId     = Uuid{145},
    .username = "player",
    .password = "password",
  };
  EXPECT_CALL(*mockAccountRepo, findOneByName("player")).Times(1).WillOnce(Return(account));

  Player player{
    .dbId       = Uuid{236},
    .account    = account.dbId,
    .name       = "player",
    .faction    = Faction::COLONIAL,
    .role       = GameRole::GUNNER,
    .systemDbId = Uuid{19},
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
  EXPECT_EQ(Uuid{19}, event->as<PlayerLoginEvent>().tryGetSystemDbId());
}

} // namespace bsgalone::core
