
#include "AccountRepository.hh"
#include "DbConnectionFixture.hh"
#include "DbQueryHelper.hh"
#include "TimeUtils.hh"
#include <gmock/gmock-matchers.h>
#include <gtest/gtest.h>

using namespace test;
using namespace ::testing;

namespace bsgalone::server {
using Integration_Bsgalone_Server_Domain_Adapters_Driven_Repositories_AccountRepository
  = DbConnectionFixture;

TEST_F(Integration_Bsgalone_Server_Domain_Adapters_Driven_Repositories_AccountRepository,
       InitializeDoesNotThrow)
{
  AccountRepository repo(this->dbConnection());

  EXPECT_NO_THROW([&repo]() { repo.initialize(); }());
}

TEST_F(Integration_Bsgalone_Server_Domain_Adapters_Driven_Repositories_AccountRepository,
       FailsWhenInitializeIsNotCalled)
{
  AccountRepository repo(this->dbConnection());

  EXPECT_THAT([&repo]() { repo.findOneByName("player"); },
              ThrowsMessage<::core::CoreException>("Failed to execute sql query"));
}

TEST_F(Integration_Bsgalone_Server_Domain_Adapters_Driven_Repositories_AccountRepository,
       FindOneByName_ReturnsEmptyOptionalWhenFetchingUnknownAccount)
{
  AccountRepository repo(this->dbConnection());
  repo.initialize();

  const auto actual = repo.findOneByName("not-a-name");

  EXPECT_FALSE(actual.has_value());
}

TEST_F(Integration_Bsgalone_Server_Domain_Adapters_Driven_Repositories_AccountRepository,
       FindOneByName_FetchesExistingAccount)
{
  AccountRepository repo(this->dbConnection());
  repo.initialize();

  const auto expectedAccount = insertTestAccount(*this->dbConnection());

  const auto actual = repo.findOneByName(expectedAccount.username);

  ASSERT_TRUE(actual.has_value());
  EXPECT_EQ(expectedAccount.dbId, actual->dbId);
  EXPECT_EQ(expectedAccount.username, actual->username);
  EXPECT_EQ(expectedAccount.password, actual->password);
}

TEST_F(Integration_Bsgalone_Server_Domain_Adapters_Driven_Repositories_AccountRepository,
       Save_InsertsNewAccount)
{
  const auto name = std::format("random-player-{:%F%T}", ::core::now());
  Account account{
    .dbId     = core::Uuid{},
    .username = name,
    .password = "secret",
  };

  AccountRepository repo(this->dbConnection());
  repo.initialize();
  repo.save(account);

  const auto maybeDbAccount = repo.findOneByName(account.username);
  ASSERT_TRUE(maybeDbAccount.has_value());
  EXPECT_EQ(account.dbId, maybeDbAccount->dbId);
  EXPECT_EQ(account.username, maybeDbAccount->username);
  EXPECT_EQ(account.password, maybeDbAccount->password);
}

TEST_F(Integration_Bsgalone_Server_Domain_Adapters_Driven_Repositories_AccountRepository,
       Save_UpdatesPasswordWhenAccountAlreadyExistsWithSameName)
{
  const auto account1 = insertTestAccount(*this->dbConnection());

  EXPECT_NE("super-secret", account1.password);
  Account account2{
    .dbId     = account1.dbId,
    .username = account1.username,
    .password = "super-secret",
  };

  AccountRepository repo(this->dbConnection());
  repo.initialize();
  repo.save(account2);

  const auto maybeDbAccount = repo.findOneByName(account1.username);
  ASSERT_TRUE(maybeDbAccount.has_value());
  EXPECT_EQ(account1.dbId, maybeDbAccount->dbId);
  EXPECT_EQ(account1.username, maybeDbAccount->username);
  EXPECT_EQ(account2.password, maybeDbAccount->password);
}

TEST_F(Integration_Bsgalone_Server_Domain_Adapters_Driven_Repositories_AccountRepository,
       Save_FailsWhenAccountAlreadyExistsWithSameName)
{
  const auto account1 = insertTestAccount(*this->dbConnection());

  Account account2{
    .username = account1.username,
    .password = "super-secret",
  };

  AccountRepository repo(this->dbConnection());
  repo.initialize();

  auto code = [&repo, &account2]() { repo.save(account2); };
  EXPECT_THAT(code, ThrowsMessage<::core::CoreException>("Failed to execute sql query"));
}

} // namespace bsgalone::server
