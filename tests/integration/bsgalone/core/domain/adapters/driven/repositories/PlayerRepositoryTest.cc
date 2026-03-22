
#include "PlayerRepository.hh"
#include "DbConnectionFixture.hh"
#include "DbQueryHelper.hh"
#include "TimeUtils.hh"
#include <gmock/gmock-matchers.h>
#include <gtest/gtest.h>

using namespace test;
using namespace ::testing;

namespace bsgalone::core {
using Integration_Bsgalone_Core_Domain_Adapters_Driven_Repositories_PlayerRepository
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

TEST_F(Integration_Bsgalone_Core_Domain_Adapters_Driven_Repositories_PlayerRepository,
       InitializeDoesNotThrow)
{
  PlayerRepository repo(this->dbConnection());

  EXPECT_NO_THROW([&repo]() { repo.initialize(); }());
}

TEST_F(Integration_Bsgalone_Core_Domain_Adapters_Driven_Repositories_PlayerRepository,
       FailsWhenInitializeIsNotCalled)
{
  PlayerRepository repo(this->dbConnection());

  EXPECT_THAT([&repo]() { repo.findOneByAccount(Uuid{269871}); },
              ThrowsMessage<::core::CoreException>("Failed to execute query returning single row"));
}

TEST_F(Integration_Bsgalone_Core_Domain_Adapters_Driven_Repositories_PlayerRepository,
       FindOneById_FetchesExistingPlayer)
{
  PlayerRepository repo(this->dbConnection());
  repo.initialize();

  const auto account        = insertTestAccount(*this->dbConnection());
  const auto expectedPlayer = insertTestPlayer(*this->dbConnection(), account.dbId, true);

  const auto actual = repo.findOneById(expectedPlayer.dbId);

  EXPECT_EQ(expectedPlayer.dbId, actual.dbId);
  EXPECT_EQ(expectedPlayer.account, actual.account);
  EXPECT_EQ(expectedPlayer.name, actual.name);
  EXPECT_EQ(expectedPlayer.faction, actual.faction);
  EXPECT_EQ(expectedPlayer.role, actual.role);
  // TODO: Verify ships
}

TEST_F(Integration_Bsgalone_Core_Domain_Adapters_Driven_Repositories_PlayerRepository,
       FindOneById_FailsWhenPlayerDoesNotDefineRole)
{
  PlayerRepository repo(this->dbConnection());
  repo.initialize();

  const auto account        = insertTestAccount(*this->dbConnection());
  const auto expectedPlayer = insertTestPlayer(*this->dbConnection(), account.dbId, false);

  const auto function = [&repo, &expectedPlayer]() { repo.findOneById(expectedPlayer.dbId); };
  EXPECT_THAT(function,
              ThrowsMessage<::core::CoreException>("Failed to execute query returning single row"));
}

TEST_F(Integration_Bsgalone_Core_Domain_Adapters_Driven_Repositories_PlayerRepository,
       FindOneById_ThrowsWhenFetchingUnknownPlayer)
{
  PlayerRepository repo(this->dbConnection());
  repo.initialize();

  EXPECT_THAT([&repo]() { repo.findOneById(Uuid{269871}); },
              ThrowsMessage<::core::CoreException>("Failed to execute query returning single row"));
}

TEST_F(Integration_Bsgalone_Core_Domain_Adapters_Driven_Repositories_PlayerRepository,
       FindOneByAccount_FetchesExistingPlayer)
{
  PlayerRepository repo(this->dbConnection());
  repo.initialize();

  const auto account        = insertTestAccount(*this->dbConnection());
  const auto expectedPlayer = insertTestPlayer(*this->dbConnection(), account.dbId, true);

  const auto actual = repo.findOneByAccount(account.dbId);

  EXPECT_EQ(expectedPlayer.dbId, actual.dbId);
  EXPECT_EQ(expectedPlayer.account, actual.account);
  EXPECT_EQ(expectedPlayer.name, actual.name);
  EXPECT_EQ(expectedPlayer.faction, actual.faction);
  EXPECT_EQ(expectedPlayer.role, actual.role);
  // TODO: Verify ships
}

TEST_F(Integration_Bsgalone_Core_Domain_Adapters_Driven_Repositories_PlayerRepository,
       FindOneByAccount_FailsWhenPlayerDoesNotDefineRole)
{
  PlayerRepository repo(this->dbConnection());
  repo.initialize();

  const auto account        = insertTestAccount(*this->dbConnection());
  const auto expectedPlayer = insertTestPlayer(*this->dbConnection(), account.dbId, false);

  const auto function = [&repo, &expectedPlayer]() {
    repo.findOneByAccount(*expectedPlayer.account);
  };
  EXPECT_THAT(function,
              ThrowsMessage<::core::CoreException>("Failed to execute query returning single row"));
}

TEST_F(Integration_Bsgalone_Core_Domain_Adapters_Driven_Repositories_PlayerRepository,
       FindOneByAccount_ThrowsWhenFetchingUnknownPlayer)
{
  PlayerRepository repo(this->dbConnection());
  repo.initialize();

  EXPECT_THAT([&repo]() { repo.findOneByAccount(Uuid{269871}); },
              ThrowsMessage<::core::CoreException>("Failed to execute query returning single row"));
}

TEST_F(Integration_Bsgalone_Core_Domain_Adapters_Driven_Repositories_PlayerRepository,
       Save_InsertsNewPlayer)
{
  PlayerRepository repo(this->dbConnection());
  repo.initialize();

  const auto account = insertTestAccount(*this->dbConnection());

  const auto name = std::format("random-player-{:%F%T}", ::core::now());
  // TODO: Should include the system
  Player player{
    .account = account.dbId,
    .name    = name,
    .faction = Faction::CYLON,
    .role    = GameRole::GUNNER,
  };

  const auto actual = repo.save(player);

  std::cout << "actual.name = " << actual.name << "\n";
  std::cout << "actual.id = " << actual.dbId << "\n";

  const auto dbPlayer = repo.findOneById(actual.dbId);
  EXPECT_EQ(player.account, dbPlayer.account);
  EXPECT_EQ(player.name, dbPlayer.name);
  EXPECT_EQ(player.faction, dbPlayer.faction);
  EXPECT_EQ(player.role, dbPlayer.role);
  // TODO: Verify ships
}

TEST_F(Integration_Bsgalone_Core_Domain_Adapters_Driven_Repositories_PlayerRepository,
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
              ThrowsMessage<::core::CoreException>("Failed to execute query returning single row"));
}

TEST_F(Integration_Bsgalone_Core_Domain_Adapters_Driven_Repositories_PlayerRepository,
       Save_UpdatesPlayerWithSameAccount)
{
  PlayerRepository repo(this->dbConnection());
  repo.initialize();

  const auto account = insertTestAccount(*this->dbConnection());
  const auto player  = insertTestPlayer(*this->dbConnection(), account.dbId, true);

  auto updatedPlayer = player;
  updatedPlayer.name = std::format("random-player-{:%F%T}", ::core::now());

  const auto actual = repo.save(updatedPlayer);

  const auto dbPlayer = repo.findOneById(actual.dbId);
  EXPECT_EQ(player.account, dbPlayer.account);
  EXPECT_EQ(player.account, dbPlayer.account);
  EXPECT_EQ(updatedPlayer.name, dbPlayer.name);
  EXPECT_EQ(player.faction, dbPlayer.faction);
  EXPECT_EQ(player.role, dbPlayer.role);
  // TODO: Verify ships
}

TEST_F(Integration_Bsgalone_Core_Domain_Adapters_Driven_Repositories_PlayerRepository,
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
