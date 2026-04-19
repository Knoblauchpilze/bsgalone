
#include "PlayerRepository.hh"
#include "DbConnectionFixture.hh"
#include "DbQueryHelper.hh"
#include "TimeUtils.hh"
#include <gmock/gmock-matchers.h>
#include <gtest/gtest.h>

using namespace test;
using namespace ::testing;

namespace bsgalone::server {
using Integration_Bsgalone_Server_Domain_Adapters_Driven_Repositories_PlayerRepository
  = DbConnectionFixture;

namespace {
void insertTestPlayerRole(DbConnection &dbConnection, Player &player)
{
  constexpr auto QUERY = R"(
      INSERT INTO player_role ("player", "role")
        VALUES ($1, 'pilot')
    )";

  const auto query = [&player](pqxx::nontransaction &work) {
    return work.exec(QUERY, pqxx::params{player.dbId.toDbId()}).no_rows();
  };
  dbConnection.executeQuery(query);

  player.role = core::GameRole::PILOT;
}

auto insertTestPlayer(DbConnection &dbConnection, const core::Uuid accountDbId, const bool withRole)
  -> Player
{
  const core::Uuid uuid;
  const auto name = std::format("random-player-{:%F%T}", ::core::now());

  constexpr auto QUERY = R"(
      INSERT INTO player ("id", "account", "name", "faction")
        VALUES ($1, $2, $3, $4)
    )";

  const auto query = [&uuid, &name, &accountDbId](pqxx::nontransaction &work) {
    return work
      .exec(QUERY,
            pqxx::params{uuid.toDbId(),
                         accountDbId.toDbId(),
                         name,
                         core::toDbFaction(core::Faction::CYLON)})
      .no_rows();
  };
  dbConnection.executeQuery(query);

  Player out{
    .dbId    = uuid,
    .account = accountDbId,
    .name    = name,
    .faction = core::Faction::CYLON,
  };

  if (withRole)
  {
    insertTestPlayerRole(dbConnection, out);
  }

  return out;
}
} // namespace

TEST_F(Integration_Bsgalone_Server_Domain_Adapters_Driven_Repositories_PlayerRepository,
       InitializeDoesNotThrow)
{
  PlayerRepository repo(this->dbConnection());

  EXPECT_NO_THROW([&repo]() { repo.initialize(); }());
}

TEST_F(Integration_Bsgalone_Server_Domain_Adapters_Driven_Repositories_PlayerRepository,
       FailsWhenInitializeIsNotCalled)
{
  PlayerRepository repo(this->dbConnection());

  EXPECT_THAT([&repo]() { repo.findOneByAccount(core::Uuid{}); },
              ThrowsMessage<::core::CoreException>(
                "Failed to execute sql query returning single row"));
}

TEST_F(Integration_Bsgalone_Server_Domain_Adapters_Driven_Repositories_PlayerRepository,
       FindOneById_ThrowsWhenFetchingUnknownPlayer)
{
  PlayerRepository repo(this->dbConnection());
  repo.initialize();

  EXPECT_THAT([&repo]() { repo.findOneById(core::Uuid{}); },
              ThrowsMessage<::core::CoreException>(
                "Failed to execute sql query returning single row"));
}

TEST_F(Integration_Bsgalone_Server_Domain_Adapters_Driven_Repositories_PlayerRepository,
       FindOneById_FailsWhenPlayerDoesNotDefineRole)
{
  const auto account        = insertTestAccount(*this->dbConnection());
  const auto expectedPlayer = insertTestPlayer(*this->dbConnection(), account.dbId, false);

  PlayerRepository repo(this->dbConnection());
  repo.initialize();
  const auto function = [&repo, &expectedPlayer]() { repo.findOneById(expectedPlayer.dbId); };
  EXPECT_THAT(function,
              ThrowsMessage<::core::CoreException>(
                "Failed to execute sql query returning single row"));
}

TEST_F(Integration_Bsgalone_Server_Domain_Adapters_Driven_Repositories_PlayerRepository,
       FindOneById_FetchesExistingPlayer)
{
  const auto account        = insertTestAccount(*this->dbConnection());
  const auto expectedPlayer = insertTestPlayer(*this->dbConnection(), account.dbId, true);

  PlayerRepository repo(this->dbConnection());
  repo.initialize();
  const auto actual = repo.findOneById(expectedPlayer.dbId);

  EXPECT_EQ(expectedPlayer.dbId, actual.dbId);
  EXPECT_EQ(expectedPlayer.account, actual.account);
  EXPECT_EQ(expectedPlayer.name, actual.name);
  EXPECT_EQ(expectedPlayer.faction, actual.faction);
  EXPECT_EQ(expectedPlayer.role, actual.role);
}

TEST_F(Integration_Bsgalone_Server_Domain_Adapters_Driven_Repositories_PlayerRepository,
       FindOneByAccount_ThrowsWhenFetchingUnknownPlayer)
{
  PlayerRepository repo(this->dbConnection());
  repo.initialize();

  EXPECT_THAT([&repo]() { repo.findOneByAccount(core::Uuid{}); },
              ThrowsMessage<::core::CoreException>(
                "Failed to execute sql query returning single row"));
}

TEST_F(Integration_Bsgalone_Server_Domain_Adapters_Driven_Repositories_PlayerRepository,
       FindOneByAccount_FailsWhenPlayerDoesNotDefineRole)
{
  const auto account        = insertTestAccount(*this->dbConnection());
  const auto expectedPlayer = insertTestPlayer(*this->dbConnection(), account.dbId, false);

  PlayerRepository repo(this->dbConnection());
  repo.initialize();
  const auto function = [&repo, &expectedPlayer]() {
    repo.findOneByAccount(*expectedPlayer.account);
  };
  EXPECT_THAT(function,
              ThrowsMessage<::core::CoreException>(
                "Failed to execute sql query returning single row"));
}

TEST_F(Integration_Bsgalone_Server_Domain_Adapters_Driven_Repositories_PlayerRepository,
       FindOneByAccount_FetchesExistingPlayer)
{
  const auto account        = insertTestAccount(*this->dbConnection());
  const auto expectedPlayer = insertTestPlayer(*this->dbConnection(), account.dbId, true);

  PlayerRepository repo(this->dbConnection());
  repo.initialize();
  const auto actual = repo.findOneByAccount(account.dbId);

  EXPECT_EQ(expectedPlayer.dbId, actual.dbId);
  EXPECT_EQ(expectedPlayer.account, actual.account);
  EXPECT_EQ(expectedPlayer.name, actual.name);
  EXPECT_EQ(expectedPlayer.faction, actual.faction);
  EXPECT_EQ(expectedPlayer.role, actual.role);
}

TEST_F(Integration_Bsgalone_Server_Domain_Adapters_Driven_Repositories_PlayerRepository,
       Save_InsertsNewPlayer)
{
  const auto account = insertTestAccount(*this->dbConnection());

  const auto name = std::format("random-player-{:%F%T}", ::core::now());
  Player player{
    .account = account.dbId,
    .name    = name,
    .faction = core::Faction::CYLON,
    .role    = core::GameRole::GUNNER,
  };

  PlayerRepository repo(this->dbConnection());
  repo.initialize();
  repo.save(player);

  const auto dbPlayer = repo.findOneById(player.dbId);
  EXPECT_EQ(player.account, dbPlayer.account);
  EXPECT_EQ(player.name, dbPlayer.name);
  EXPECT_EQ(player.faction, dbPlayer.faction);
  EXPECT_EQ(player.role, dbPlayer.role);
}

TEST_F(Integration_Bsgalone_Server_Domain_Adapters_Driven_Repositories_PlayerRepository,
       Save_FailsWhenPlayerAlreadyExistsWithSameName)
{
  const auto account1 = insertTestAccount(*this->dbConnection());
  const auto account2 = insertTestAccount(*this->dbConnection());

  const auto player1 = insertTestPlayer(*this->dbConnection(), account1.dbId, true);
  Player player2{
    .account = account2.dbId,
    .name    = player1.name,
    .faction = core::Faction::CYLON,
    .role    = core::GameRole::GUNNER,
  };

  PlayerRepository repo(this->dbConnection());
  repo.initialize();

  auto code = [&repo, &player2]() { repo.save(player2); };
  EXPECT_THAT(code, ThrowsMessage<::core::CoreException>("Failed to save player: 23505"));
}

TEST_F(Integration_Bsgalone_Server_Domain_Adapters_Driven_Repositories_PlayerRepository,
       Save_FailsWhenPlayerAlreadyExistsWithSameAccount)
{
  const auto account1 = insertTestAccount(*this->dbConnection());

  const auto player1 = insertTestPlayer(*this->dbConnection(), account1.dbId, true);
  Player player2{
    .account = account1.dbId,
    .name    = std::format("random-player-{:%F%T}", ::core::now()),
    .faction = core::Faction::CYLON,
    .role    = core::GameRole::GUNNER,
  };

  PlayerRepository repo(this->dbConnection());
  repo.initialize();

  auto code = [&repo, &player2]() { repo.save(player2); };
  EXPECT_THAT(code, ThrowsMessage<::core::CoreException>("Failed to save player: 23505"));
}

TEST_F(Integration_Bsgalone_Server_Domain_Adapters_Driven_Repositories_PlayerRepository,
       Save_UpdatesNameWhenPlayerAlreadyExists)
{
  const auto account = insertTestAccount(*this->dbConnection());
  const auto player  = insertTestPlayer(*this->dbConnection(), account.dbId, true);

  auto updatedPlayer = player;
  updatedPlayer.name = std::format("random-player-{:%F%T}", ::core::now());

  PlayerRepository repo(this->dbConnection());
  repo.initialize();
  repo.save(updatedPlayer);

  const auto dbPlayer = repo.findOneById(player.dbId);
  EXPECT_EQ(updatedPlayer.name, dbPlayer.name);
}

TEST_F(Integration_Bsgalone_Server_Domain_Adapters_Driven_Repositories_PlayerRepository,
       Save_UpdatesRoleWhenPlayerAlreadyExists)
{
  const auto account = insertTestAccount(*this->dbConnection());
  const auto player  = insertTestPlayer(*this->dbConnection(), account.dbId, true);

  auto updatedPlayer = player;
  ASSERT_NE(core::GameRole::GUNNER, player.role);
  updatedPlayer.role = core::GameRole::GUNNER;

  PlayerRepository repo(this->dbConnection());
  repo.initialize();
  repo.save(updatedPlayer);

  const auto dbPlayer = repo.findOneById(player.dbId);
  EXPECT_EQ(updatedPlayer.role, dbPlayer.role);
}

TEST_F(Integration_Bsgalone_Server_Domain_Adapters_Driven_Repositories_PlayerRepository,
       Save_DoesNotUpdateOtherPlayersOrRoles)
{
  PlayerRepository repo(this->dbConnection());
  repo.initialize();

  const auto account1 = insertTestAccount(*this->dbConnection());
  Player player1{
    .account = account1.dbId,
    .name    = std::format("random-player-{:%F%T}", ::core::now()),
    .faction = core::Faction::CYLON,
    .role    = core::GameRole::PILOT,
  };
  repo.save(player1);

  const auto account2 = insertTestAccount(*this->dbConnection());
  Player player2{
    .account = account2.dbId,
    .name    = std::format("random-player-{:%F%T}", ::core::now()),
    .faction = core::Faction::COLONIAL,
    .role    = core::GameRole::GUNNER,
  };
  repo.save(player2);

  const auto actual = repo.findOneById(player1.dbId);
  EXPECT_EQ(core::GameRole::PILOT, actual.role);
}

} // namespace bsgalone::server
