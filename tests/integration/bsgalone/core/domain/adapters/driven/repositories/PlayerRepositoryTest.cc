
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
auto insertTestPlayer(DbConnection &dbConnection, const Uuid accountDbId) -> Player
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
       FetchesExistingPlayer)
{
  PlayerRepository repo(this->dbConnection());
  repo.initialize();

  const auto account        = insertTestAccount(*this->dbConnection());
  const auto expectedPlayer = insertTestPlayer(*this->dbConnection(), account.dbId);

  const auto actual = repo.findOneByAccount(account.dbId);

  EXPECT_EQ(expectedPlayer.dbId, actual.dbId);
  EXPECT_EQ(expectedPlayer.account, actual.account);
  EXPECT_EQ(expectedPlayer.name, actual.name);
  EXPECT_EQ(expectedPlayer.faction, actual.faction);
}

TEST_F(Integration_Bsgalone_Core_Domain_Adapters_Driven_Repositories_PlayerRepository,
       FailsWhenInitializeIsNotCalled)
{
  PlayerRepository repo(this->dbConnection());

  EXPECT_THAT([&repo]() { repo.findOneByAccount(Uuid{269871}); },
              ThrowsMessage<::core::CoreException>("Failed to execute query returning single row"));
}

TEST_F(Integration_Bsgalone_Core_Domain_Adapters_Driven_Repositories_PlayerRepository,
       ThrowsWhenFetchingUnknownPlayer)
{
  PlayerRepository repo(this->dbConnection());
  repo.initialize();

  EXPECT_THAT([&repo]() { repo.findOneByAccount(Uuid{269871}); },
              ThrowsMessage<::core::CoreException>("Failed to execute query returning single row"));
}

} // namespace bsgalone::core
