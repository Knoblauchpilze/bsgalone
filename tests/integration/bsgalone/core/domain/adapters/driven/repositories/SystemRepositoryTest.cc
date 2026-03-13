
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
void insertTestTickConfig(DbConnection &dbConnection, System &system)
{
  constexpr auto QUERY = R"(
      INSERT INTO tick_config ("system", "duration", "unit", "ticks")
        VALUES ($1, 1, 'seconds', 14)
    )";

  const auto query = [&system](pqxx::nontransaction &work) {
    return work.exec(QUERY, pqxx::params{toDbId(system.dbId)}).no_rows();
  };
  dbConnection.executeQuery(query);

  system.step = chrono::TimeStep(14, chrono::Duration::fromSeconds(1.0f));
}

void insertTestTick(DbConnection &dbConnection, System &system)
{
  constexpr auto QUERY = R"(
      INSERT INTO tick ("system", "current_tick")
        VALUES ($1, 27)
    )";

  const auto query = [&system](pqxx::nontransaction &work) {
    return work.exec(QUERY, pqxx::params{toDbId(system.dbId)}).no_rows();
  };
  dbConnection.executeQuery(query);

  system.currentTick = chrono::Tick::fromInt(27);
}

auto insertTestSystem(DbConnection &dbConnection, const bool withTick) -> System
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

  System out{
    .dbId     = fromDbId(record[0].as<int>()),
    .name     = name,
    .position = Eigen::Vector3f(2.5f, -1.2f, 3.4f),
  };

  if (withTick)
  {
    insertTestTickConfig(dbConnection, out);
    insertTestTick(dbConnection, out);
  }

  return out;
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

  const auto expectedSystem = insertTestSystem(*this->dbConnection(), true);

  const auto actual = repo.findOneById(expectedSystem.dbId);

  EXPECT_EQ(expectedSystem, actual);
}

TEST_F(Integration_Bsgalone_Core_Domain_Adapters_Driven_Repositories_SystemRepository,
       FailsWhenSystemDoesNotDefineTick)
{
  SystemRepository repo(this->dbConnection());
  repo.initialize();

  const auto expectedSystem = insertTestSystem(*this->dbConnection(), false);

  const auto function = [&repo, &expectedSystem]() { repo.findOneById(expectedSystem.dbId); };
  EXPECT_THAT(function,
              ThrowsMessage<::core::CoreException>("Failed to execute query returning single row"));
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
