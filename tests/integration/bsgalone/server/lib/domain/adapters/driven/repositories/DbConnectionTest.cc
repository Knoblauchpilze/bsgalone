
#include "DbConnection.hh"
#include "DbConnectionFixture.hh"
#include "TimeUtils.hh"
#include "Uuid.hh"
#include <gmock/gmock-matchers.h>
#include <gtest/gtest.h>

using namespace test;
using namespace ::testing;

namespace bsgalone::core {
using Integration_Bsgalone_Server_Domain_Adapters_Driven_Repositories_DbConnection
  = DbConnectionFixture;

// TODO: Add tests for disconnection

TEST_F(Integration_Bsgalone_Server_Domain_Adapters_Driven_Repositories_DbConnection,
       ThrowsOnValidSqlWhenDisconnected)
{
  auto code = [this]() {
    const auto query = [](pqxx::nontransaction &work) {
      return work.exec("SELECT COUNT(*) FROM account").one_row();
    };

    this->dbConnection()->executeQueryReturningSingleRow(query);
  };

  EXPECT_NO_THROW(code());

  this->dbConnection()->disconnect();

  EXPECT_THAT(code,
              ThrowsMessage<::core::CoreException>(
                "Not connected, please call connect before executing sql queries"));
}

TEST_F(Integration_Bsgalone_Server_Domain_Adapters_Driven_Repositories_DbConnection,
       ExecuteQuery_ThrowsOnSqlError)
{
  const auto query = [](pqxx::nontransaction &work) {
    return work.exec("SELECT id FROM table_that_does_not_exist");
  };

  auto code = [this, &query]() { this->dbConnection()->executeQuery(query); };
  EXPECT_THAT(code, ThrowsMessage<::core::CoreException>("Failed to execute sql query"));
}

TEST_F(Integration_Bsgalone_Server_Domain_Adapters_Driven_Repositories_DbConnection,
       ExecuteQueryReturningSingleRow_ThrowsWhenNoRowsAreReturned)
{
  const auto query = [](pqxx::nontransaction &work) {
    return work.exec("SELECT id FROM account WHERE name = 'not-existing-account'").one_row();
  };

  auto code = [this, &query]() { this->dbConnection()->executeQueryReturningSingleRow(query); };
  EXPECT_THAT(code,
              ThrowsMessage<::core::CoreException>(
                "Failed to execute sql query returning single row"));
}

TEST_F(Integration_Bsgalone_Server_Domain_Adapters_Driven_Repositories_DbConnection,
       TryExecuteTransaction_CommitsWhenSuccessful)
{
  const auto username = std::format("random-account-{:%F%T}", ::core::now());

  const auto insertQuery = [&username](pqxx::work &transaction) {
    constexpr auto INSERT_QUERY = R"(
      INSERT INTO account ("name", "password")
        VALUES ($1, 'password')
        RETURNING id
    )";

    return transaction.exec(INSERT_QUERY, pqxx::params{username});
  };

  const auto out = this->dbConnection()->tryExecuteTransaction(insertQuery);

  ASSERT_FALSE(out.error.has_value());
  ASSERT_TRUE(out.result.has_value());
  ASSERT_FALSE(out.result->empty());
  ASSERT_EQ(1, out.result->size());

  const auto &record      = (*out.result)[0];
  const auto expectedDbId = fromDbId(record[0].as<int>());

  const auto fetchQuery = [&username](pqxx::nontransaction &work) {
    constexpr auto FETCH_QUERY = R"(
      SELECT
        id, password
      FROM
        account
      WHERE
        name = $1
    )";

    return work.exec(FETCH_QUERY, pqxx::params{username}).one_row();
  };

  const auto accountRecord = this->dbConnection()->executeQueryReturningSingleRow(fetchQuery);
  EXPECT_EQ(expectedDbId, fromDbId(accountRecord[0].as<int>()));
  EXPECT_EQ("password", accountRecord[1].as<std::string>());
}

TEST_F(Integration_Bsgalone_Server_Domain_Adapters_Driven_Repositories_DbConnection,
       TryExecuteTransaction_RollsBackWhenAStatementFails)
{
  const auto username = std::format("random-account-{:%F%T}", ::core::now());

  const auto insertQuery = [&username](pqxx::work &transaction) {
    constexpr auto INSERT_QUERY = R"(
      INSERT INTO account ("name", "password")
        VALUES ($1, 'password')
        RETURNING id
    )";

    transaction.exec(INSERT_QUERY, pqxx::params{username});

    // Second insert should fail due to unique constraint
    return transaction.exec(INSERT_QUERY, pqxx::params{username});
  };

  const auto out = this->dbConnection()->tryExecuteTransaction(insertQuery);
  ASSERT_TRUE(out.error.has_value());
  EXPECT_EQ("23505", *out.error);
  EXPECT_FALSE(out.result.has_value());

  const auto fetchQuery = [&username](pqxx::nontransaction &work) {
    constexpr auto FETCH_QUERY = R"(
      SELECT COUNT(*) FROM account WHERE name = $1
    )";

    return work.exec(FETCH_QUERY, pqxx::params{username}).one_row();
  };

  const auto record = this->dbConnection()->executeQueryReturningSingleRow(fetchQuery);
  EXPECT_EQ(record[0].as<int>(), 0);
}

} // namespace bsgalone::core
