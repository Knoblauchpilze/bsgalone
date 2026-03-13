
#include "SystemRepository.hh"
#include "DbConnectionFixture.hh"
#include "TimeUtils.hh"
#include <gmock/gmock-matchers.h>
#include <gtest/gtest.h>

using namespace test;
using namespace ::testing;

namespace bsgalone::core {
using Integration_Bsgalone_Core_Domain_Adapters_Driven_Repositories_SystemRepository
  = DbConnectionFixture;

namespace {
auto insertTestSystem(DbConnection &dbConnection) -> System
{
  // https://stackoverflow.com/questions/34857119/how-to-convert-stdchronotime-point-to-string
  // https://en.cppreference.com/w/cpp/chrono/system_clock/formatter.html
  const auto name = std::format("random-system-{:%F%T}", ::core::now());

  constexpr auto QUERY = R"(
      INSERT INTO system ("name", "x_pos", "y_pos", "z_pos")
        VALUES ($1, 2.5, -1.2, 3.4)
        RETURNING id
    )";

  const auto query = [&name](pqxx::nontransaction &work) {
    return work.exec(QUERY, pqxx::params{name}).one_row();
  };
  auto record = dbConnection.executeQueryReturningSingleRow(query);

  return System{.dbId     = fromDbId(record[0].as<int>()),
                .name     = name,
                .position = Eigen::Vector3f(2.5f, -1.2f, 3.4f)};
}
} // namespace

TEST_F(Integration_Bsgalone_Core_Domain_Adapters_Driven_Repositories_SystemRepository,
       InitializeDoesNotThrow)
{
  SystemRepository repo(this->dbConnection());

  EXPECT_NO_THROW([&repo]() { repo.initialize(); }());
}

TEST_F(Integration_Bsgalone_Core_Domain_Adapters_Driven_Repositories_SystemRepository,
       FetchesExistingSystem)
{
  SystemRepository repo(this->dbConnection());
  repo.initialize();

  const auto expectedSystem = insertTestSystem(*this->dbConnection());

  const auto actual = repo.findOneById(expectedSystem.dbId);

  EXPECT_EQ(expectedSystem, actual);
}

TEST_F(Integration_Bsgalone_Core_Domain_Adapters_Driven_Repositories_SystemRepository,
       FailsWhenInitializeIsNotCalled)
{
  SystemRepository repo(this->dbConnection());

  EXPECT_THAT([&repo]() { repo.findOneById(Uuid{269871}); },
              ThrowsMessage<::core::CoreException>("Failed to execute query returning single row"));
}

TEST_F(Integration_Bsgalone_Core_Domain_Adapters_Driven_Repositories_SystemRepository,
       ThrowsWhenFetchingUnknownSystem)
{
  SystemRepository repo(this->dbConnection());
  repo.initialize();

  EXPECT_THAT([&repo]() { repo.findOneById(Uuid{269871}); },
              ThrowsMessage<::core::CoreException>("Failed to execute query returning single row"));
}

} // namespace bsgalone::core
