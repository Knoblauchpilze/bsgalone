
#include "AccountRepository.hh"
#include "DbConnectionFixture.hh"
#include "TimeUtils.hh"
#include <gmock/gmock-matchers.h>
#include <gtest/gtest.h>

using namespace test;
using namespace ::testing;

namespace bsgalone::core {
using Integration_Bsgalone_Core_Domain_Adapters_Driven_Repositories_AccountRepository
  = DbConnectionFixture;

namespace {
auto insertTestAccount(DbConnection &dbConnection) -> Account
{
  const auto username = std::format("random-account-{:%F%T}", ::core::now());

  constexpr auto QUERY = R"(
      INSERT INTO account ("name", "password")
        VALUES ($1, 'password')
        RETURNING id
    )";

  const auto query = [&username](pqxx::nontransaction &work) {
    return work.exec(QUERY, pqxx::params{username}).one_row();
  };
  auto record = dbConnection.executeQueryReturningSingleRow(query);

  Account out{
    .dbId     = fromDbId(record[0].as<int>()),
    .username = username,
    .password = "password",
  };

  return out;
}

} // namespace

TEST_F(Integration_Bsgalone_Core_Domain_Adapters_Driven_Repositories_AccountRepository,
       InitializeDoesNotThrow)
{
  AccountRepository repo(this->dbConnection());

  EXPECT_NO_THROW([&repo]() { repo.initialize(); }());
}

TEST_F(Integration_Bsgalone_Core_Domain_Adapters_Driven_Repositories_AccountRepository,
       FetchesExistingAccount)
{
  AccountRepository repo(this->dbConnection());
  repo.initialize();

  const auto expectedAccount = insertTestAccount(*this->dbConnection());

  const auto actual = repo.findOneByName(expectedAccount.username);

  EXPECT_EQ(expectedAccount, actual);
}

TEST_F(Integration_Bsgalone_Core_Domain_Adapters_Driven_Repositories_AccountRepository,
       FailsWhenInitializeIsNotCalled)
{
  AccountRepository repo(this->dbConnection());

  EXPECT_THAT([&repo]() { repo.findOneByName("player"); },
              ThrowsMessage<::core::CoreException>("Failed to execute query"));
}

TEST_F(Integration_Bsgalone_Core_Domain_Adapters_Driven_Repositories_AccountRepository,
       ReturnsEmptyOptionalWhenFetchingUnknownAccount)
{
  AccountRepository repo(this->dbConnection());
  repo.initialize();

  const auto actual = repo.findOneByName("not-a-name");

  EXPECT_FALSE(actual.has_value());
}

} // namespace bsgalone::core
