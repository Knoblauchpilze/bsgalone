
#include "PlayerRepository.hh"
#include "DbConnectionFixture.hh"
#include "DbQueryHelper.hh"
#include "TimeUtils.hh"
#include <gmock/gmock-matchers.h>
#include <gtest/gtest.h>

using namespace test;
using namespace ::testing;

namespace bsgalone::core {
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
    return work.exec(QUERY, pqxx::params{toDbId(player.dbId)}).no_rows();
  };
  dbConnection.executeQuery(query);

  player.role = GameRole::PILOT;
}

void insertTestPlayerShip(DbConnection &dbConnection, const Player &player, const Uuid systemDbId)
{
  const auto name = std::format("random-ship-{:%F%T}", ::core::now());

  constexpr auto QUERY_SHIP = R"(
      INSERT INTO player_ship ("ship", "player", "name", "active",
        "hull_points", "power_points", "x_pos", "y_pos", "z_pos")
        VALUES (1, $1, $2, true, 100.0, 100.0, 0.0, 0.0, 0.0)
        RETURNING id
    )";

  auto queryShip = [&name, &player](pqxx::nontransaction &work) {
    return work.exec(QUERY_SHIP, pqxx::params{toDbId(player.dbId), name}).one_row();
  };
  const auto record         = dbConnection.executeQueryReturningSingleRow(queryShip);
  const auto playerShipDbId = fromDbId(record[0].as<int>());

  constexpr auto QUERY_SYSTEM = R"(
      INSERT INTO ship_system ("ship", "system", "docked")
        VALUES ($1, $2, true)
    )";

  auto querySystem = [playerShipDbId, systemDbId](pqxx::nontransaction &work) {
    return work.exec(QUERY_SYSTEM, pqxx::params{toDbId(playerShipDbId), toDbId(systemDbId)})
      .no_rows();
  };
  dbConnection.executeQuery(querySystem);
}

auto insertTestPlayer(DbConnection &dbConnection, const Uuid accountDbId, const bool withRole)
  -> Player
{
  const auto name = std::format("random-player-{:%F%T}", ::core::now());

  constexpr auto QUERY = R"(
      INSERT INTO player ("account", "name", "faction")
        VALUES ($1, $2, $3)
        RETURNING id
    )";

  const auto query = [&name, &accountDbId](pqxx::nontransaction &work) {
    return work.exec(QUERY, pqxx::params{toDbId(accountDbId), name, toDbFaction(Faction::CYLON)})
      .one_row();
  };
  auto record = dbConnection.executeQueryReturningSingleRow(query);

  Player out{
    .dbId    = fromDbId(record[0].as<int>()),
    .account = accountDbId,
    .name    = name,
    .faction = Faction::CYLON,
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

  EXPECT_THAT([&repo]() { repo.findOneByAccount(Uuid{269871}); },
              ThrowsMessage<::core::CoreException>(
                "Failed to execute sql query returning single row"));
}

TEST_F(Integration_Bsgalone_Server_Domain_Adapters_Driven_Repositories_PlayerRepository,
       FindOneById_FetchesExistingPlayer)
{
  PlayerRepository repo(this->dbConnection());
  repo.initialize();

  const auto account        = insertTestAccount(*this->dbConnection());
  const auto expectedPlayer = insertTestPlayer(*this->dbConnection(), account.dbId, true);
  insertTestPlayerShip(*this->dbConnection(), expectedPlayer, Uuid{1});

  const auto actual = repo.findOneById(expectedPlayer.dbId);

  EXPECT_EQ(expectedPlayer.dbId, actual.dbId);
  EXPECT_EQ(expectedPlayer.account, actual.account);
  EXPECT_EQ(expectedPlayer.name, actual.name);
  EXPECT_EQ(expectedPlayer.faction, actual.faction);
  EXPECT_EQ(expectedPlayer.role, actual.role);
  EXPECT_EQ(Uuid{1}, actual.systemDbId);
}

TEST_F(Integration_Bsgalone_Server_Domain_Adapters_Driven_Repositories_PlayerRepository,
       FindOneById_FailsWhenPlayerDoesNotDefineRole)
{
  PlayerRepository repo(this->dbConnection());
  repo.initialize();

  const auto account        = insertTestAccount(*this->dbConnection());
  const auto expectedPlayer = insertTestPlayer(*this->dbConnection(), account.dbId, false);

  const auto function = [&repo, &expectedPlayer]() { repo.findOneById(expectedPlayer.dbId); };
  EXPECT_THAT(function,
              ThrowsMessage<::core::CoreException>(
                "Failed to execute sql query returning single row"));
}

TEST_F(Integration_Bsgalone_Server_Domain_Adapters_Driven_Repositories_PlayerRepository,
       FindOneById_ThrowsWhenFetchingUnknownPlayer)
{
  PlayerRepository repo(this->dbConnection());
  repo.initialize();

  EXPECT_THAT([&repo]() { repo.findOneById(Uuid{269871}); },
              ThrowsMessage<::core::CoreException>(
                "Failed to execute sql query returning single row"));
}

TEST_F(Integration_Bsgalone_Server_Domain_Adapters_Driven_Repositories_PlayerRepository,
       FindOneByAccount_FetchesExistingPlayer)
{
  PlayerRepository repo(this->dbConnection());
  repo.initialize();

  const auto account        = insertTestAccount(*this->dbConnection());
  const auto expectedPlayer = insertTestPlayer(*this->dbConnection(), account.dbId, true);
  insertTestPlayerShip(*this->dbConnection(), expectedPlayer, Uuid{1});

  const auto actual = repo.findOneByAccount(account.dbId);

  EXPECT_EQ(expectedPlayer.dbId, actual.dbId);
  EXPECT_EQ(expectedPlayer.account, actual.account);
  EXPECT_EQ(expectedPlayer.name, actual.name);
  EXPECT_EQ(expectedPlayer.faction, actual.faction);
  EXPECT_EQ(expectedPlayer.role, actual.role);
  EXPECT_EQ(Uuid{1}, actual.systemDbId);
}

TEST_F(Integration_Bsgalone_Server_Domain_Adapters_Driven_Repositories_PlayerRepository,
       FindOneByAccount_FailsWhenPlayerDoesNotDefineRole)
{
  PlayerRepository repo(this->dbConnection());
  repo.initialize();

  const auto account        = insertTestAccount(*this->dbConnection());
  const auto expectedPlayer = insertTestPlayer(*this->dbConnection(), account.dbId, false);
  insertTestPlayerShip(*this->dbConnection(), expectedPlayer, Uuid{0});

  const auto function = [&repo, &expectedPlayer]() {
    repo.findOneByAccount(*expectedPlayer.account);
  };
  EXPECT_THAT(function,
              ThrowsMessage<::core::CoreException>(
                "Failed to execute sql query returning single row"));
}

TEST_F(Integration_Bsgalone_Server_Domain_Adapters_Driven_Repositories_PlayerRepository,
       FindOneByAccount_FailsWhenPlayerDoesNotHaveActiveShip)
{
  PlayerRepository repo(this->dbConnection());
  repo.initialize();

  const auto account        = insertTestAccount(*this->dbConnection());
  const auto expectedPlayer = insertTestPlayer(*this->dbConnection(), account.dbId, true);

  const auto function = [&repo, &expectedPlayer]() {
    repo.findOneByAccount(*expectedPlayer.account);
  };
  EXPECT_THAT(function,
              ThrowsMessage<::core::CoreException>(
                "Failed to execute sql query returning single row"));
}

TEST_F(Integration_Bsgalone_Server_Domain_Adapters_Driven_Repositories_PlayerRepository,
       FindOneByAccount_ThrowsWhenFetchingUnknownPlayer)
{
  PlayerRepository repo(this->dbConnection());
  repo.initialize();

  EXPECT_THAT([&repo]() { repo.findOneByAccount(Uuid{269871}); },
              ThrowsMessage<::core::CoreException>(
                "Failed to execute sql query returning single row"));
}

TEST_F(Integration_Bsgalone_Server_Domain_Adapters_Driven_Repositories_PlayerRepository,
       Save_InsertsNewPlayer)
{
  PlayerRepository repo(this->dbConnection());
  repo.initialize();

  const auto account = insertTestAccount(*this->dbConnection());

  const auto name = std::format("random-player-{:%F%T}", ::core::now());
  Player player{
    .account = account.dbId,
    .name    = name,
    .faction = Faction::CYLON,
    .role    = GameRole::GUNNER,
  };

  const auto actual = repo.save(player);

  // TODO: This is currently needed because the player does not define
  // the ships but requires them to fetch the system.
  insertTestPlayerShip(*this->dbConnection(), actual, Uuid{0});

  const auto dbPlayer = repo.findOneById(actual.dbId);
  EXPECT_EQ(player.account, dbPlayer.account);
  EXPECT_EQ(player.name, dbPlayer.name);
  EXPECT_EQ(player.faction, dbPlayer.faction);
  EXPECT_EQ(player.role, dbPlayer.role);
}

TEST_F(Integration_Bsgalone_Server_Domain_Adapters_Driven_Repositories_PlayerRepository,
       Save_DoesNotUpdateSystem)
{
  PlayerRepository repo(this->dbConnection());
  repo.initialize();

  const auto account = insertTestAccount(*this->dbConnection());
  auto player        = insertTestPlayer(*this->dbConnection(), account.dbId, true);
  insertTestPlayerShip(*this->dbConnection(), player, Uuid{0});

  player.systemDbId = Uuid{1};

  const auto actual = repo.save(player);

  const auto dbPlayer = repo.findOneById(actual.dbId);
  EXPECT_EQ(Uuid{0}, dbPlayer.systemDbId);
}

TEST_F(Integration_Bsgalone_Server_Domain_Adapters_Driven_Repositories_PlayerRepository,
       Save_FailsWhenPlayerAlreadyExistsWithSameName)
{
  PlayerRepository repo(this->dbConnection());
  repo.initialize();

  const auto account1 = insertTestAccount(*this->dbConnection());
  const auto player1  = insertTestPlayer(*this->dbConnection(), account1.dbId, true);

  const auto account2 = insertTestAccount(*this->dbConnection());
  Player player2{
    .account = account2.dbId,
    .name    = player1.name,
    .faction = Faction::CYLON,
    .role    = GameRole::GUNNER,
  };

  auto code = [&repo, &player2]() { repo.save(player2); };
  EXPECT_THAT(code,
              ThrowsMessage<::core::CoreException>(
                "Failed to execute sql query returning single row"));
}

TEST_F(Integration_Bsgalone_Server_Domain_Adapters_Driven_Repositories_PlayerRepository,
       Save_UpdatesPlayerWithSameAccount)
{
  PlayerRepository repo(this->dbConnection());
  repo.initialize();

  const auto account = insertTestAccount(*this->dbConnection());
  const auto player  = insertTestPlayer(*this->dbConnection(), account.dbId, true);
  insertTestPlayerShip(*this->dbConnection(), player, Uuid{1});

  auto updatedPlayer = player;
  updatedPlayer.name = std::format("random-player-{:%F%T}", ::core::now());

  const auto actual = repo.save(updatedPlayer);

  const auto dbPlayer = repo.findOneById(actual.dbId);
  EXPECT_EQ(player.account, dbPlayer.account);
  EXPECT_EQ(player.account, dbPlayer.account);
  EXPECT_EQ(updatedPlayer.name, dbPlayer.name);
  EXPECT_EQ(player.faction, dbPlayer.faction);
  EXPECT_EQ(player.role, dbPlayer.role);
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
    .faction = Faction::CYLON,
    .role    = GameRole::PILOT,
  };
  player1 = repo.save(player1);
  insertTestPlayerShip(*this->dbConnection(), player1, Uuid{1});

  const auto account2 = insertTestAccount(*this->dbConnection());
  Player player2{
    .account = account2.dbId,
    .name    = std::format("random-player-{:%F%T}", ::core::now()),
    .faction = Faction::COLONIAL,
    .role    = GameRole::GUNNER,
  };
  player2 = repo.save(player2);

  const auto actual = repo.findOneById(player1.dbId);
  EXPECT_EQ(GameRole::PILOT, actual.role);
}

} // namespace bsgalone::core
