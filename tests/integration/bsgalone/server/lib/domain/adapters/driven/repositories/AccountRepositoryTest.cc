
#include "AccountRepository.hh"
#include "DbConnectionFixture.hh"
#include "DbQueryHelper.hh"
#include "TimeUtils.hh"
#include <gmock/gmock-matchers.h>
#include <gtest/gtest.h>

using namespace test;
using namespace ::testing;

namespace bsgalone::core {
using Integration_Bsgalone_Server_Domain_Adapters_Driven_Repositories_AccountRepository
  = DbConnectionFixture;

TEST_F(Integration_Bsgalone_Server_Domain_Adapters_Driven_Repositories_AccountRepository,
       InitializeDoesNotThrow)
{
  AccountRepository repo(this->dbConnection());

  EXPECT_NO_THROW([&repo]() { repo.initialize(); }());
}

TEST_F(Integration_Bsgalone_Server_Domain_Adapters_Driven_Repositories_AccountRepository,
       FetchesExistingAccount)
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
       FailsWhenInitializeIsNotCalled)
{
  AccountRepository repo(this->dbConnection());

  EXPECT_THAT([&repo]() { repo.findOneByName("player"); },
              ThrowsMessage<::core::CoreException>("Failed to execute query"));
}

TEST_F(Integration_Bsgalone_Server_Domain_Adapters_Driven_Repositories_AccountRepository,
       ReturnsEmptyOptionalWhenFetchingUnknownAccount)
{
  AccountRepository repo(this->dbConnection());
  repo.initialize();

  const auto actual = repo.findOneByName("not-a-name");

  EXPECT_FALSE(actual.has_value());
}

} // namespace bsgalone::core
