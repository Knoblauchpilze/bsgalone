
#include "SystemRepository.hh"
#include "DbConnectionFixture.hh"
#include "TimeUtils.hh"
#include <gmock/gmock-matchers.h>
#include <gtest/gtest.h>

using namespace test;
using namespace ::testing;

namespace bsgalone::server {
using Integration_Bsgalone_Server_Domain_Adapters_Driven_Repositories_SystemRepository
  = DbConnectionFixture;

namespace {
void insertTestTickConfig(DbConnection &dbConnection, System &system)
{
  constexpr auto QUERY = R"(
      INSERT INTO tick_config ("system", "duration", "unit", "ticks")
        VALUES ($1, 1, 'seconds', 14)
    )";

  const auto query = [&system](pqxx::nontransaction &work) {
    return work.exec(QUERY, pqxx::params{system.dbId.toDbId()}).no_rows();
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
    return work.exec(QUERY, pqxx::params{system.dbId.toDbId()}).no_rows();
  };
  dbConnection.executeQuery(query);

  system.currentTick = chrono::Tick::fromInt(27);
}

auto insertTestSystem(DbConnection &dbConnection, const bool withTick) -> System
{
  const core::Uuid uuid;
  // https://stackoverflow.com/questions/34857119/how-to-convert-stdchronotime-point-to-string
  // https://en.cppreference.com/w/cpp/chrono/system_clock/formatter.html
  const auto name = std::format("random-system-{:%F%T}", ::core::now());

  constexpr auto QUERY = R"(
      INSERT INTO system ("id", "name", "x_pos", "y_pos", "z_pos")
        VALUES ($1, $2, 2.5, -1.2, 3.4)
    )";

  const auto query = [&uuid, &name](pqxx::nontransaction &work) {
    return work.exec(QUERY, pqxx::params{uuid.toDbId(), name});
  };
  dbConnection.executeQuery(query);

  System out{
    .dbId     = uuid,
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

TEST_F(Integration_Bsgalone_Server_Domain_Adapters_Driven_Repositories_SystemRepository,
       InitializeDoesNotThrow)
{
  SystemRepository repo(this->dbConnection());

  EXPECT_NO_THROW([&repo]() { repo.initialize(); }());
}

TEST_F(Integration_Bsgalone_Server_Domain_Adapters_Driven_Repositories_SystemRepository,
       FindOneById_FailsWhenInitializeIsNotCalled)
{
  SystemRepository repo(this->dbConnection());

  EXPECT_THAT([&repo]() { repo.findOneById(core::Uuid{}); },
              ThrowsMessage<::core::CoreException>(
                "Failed to execute sql query returning single row"));
}

TEST_F(Integration_Bsgalone_Server_Domain_Adapters_Driven_Repositories_SystemRepository,
       FindOneById_ThrowsWhenFetchingUnknownSystem)
{
  SystemRepository repo(this->dbConnection());
  repo.initialize();

  EXPECT_THAT([&repo]() { repo.findOneById(core::Uuid{}); },
              ThrowsMessage<::core::CoreException>(
                "Failed to execute sql query returning single row"));
}

TEST_F(Integration_Bsgalone_Server_Domain_Adapters_Driven_Repositories_SystemRepository,
       FindOneById_FailsWhenSystemDoesNotDefineTick)
{
  const auto expectedSystem = insertTestSystem(*this->dbConnection(), false);

  SystemRepository repo(this->dbConnection());
  repo.initialize();
  const auto function = [&repo, &expectedSystem]() { repo.findOneById(expectedSystem.dbId); };
  EXPECT_THAT(function,
              ThrowsMessage<::core::CoreException>(
                "Failed to execute sql query returning single row"));
}

TEST_F(Integration_Bsgalone_Server_Domain_Adapters_Driven_Repositories_SystemRepository,
       FindOneById_FetchesExistingSystem)
{
  const auto expectedSystem = insertTestSystem(*this->dbConnection(), true);

  SystemRepository repo(this->dbConnection());
  repo.initialize();
  const auto actual = repo.findOneById(expectedSystem.dbId);

  EXPECT_EQ(expectedSystem.dbId, actual.dbId);
  EXPECT_EQ(expectedSystem.name, actual.name);
  EXPECT_EQ(expectedSystem.position, actual.position);
  EXPECT_EQ(expectedSystem.currentTick, actual.currentTick);
  EXPECT_EQ(expectedSystem.step, actual.step);
}

TEST_F(Integration_Bsgalone_Server_Domain_Adapters_Driven_Repositories_SystemRepository,
       FindAll_ReturnsAllExistingSystems)
{
  const auto system1 = insertTestSystem(*this->dbConnection(), true);
  const auto system2 = insertTestSystem(*this->dbConnection(), true);

  SystemRepository repo(this->dbConnection());
  repo.initialize();
  const auto allSystems = repo.findAll();

  // The other tests also insert systems so there might be more than the two
  // inseretd in this one. This could be solved by isolated test DB but is
  // fine for now.
  EXPECT_LE(2u, allSystems.size());

  const auto maybeSystem1 = std::find_if(allSystems.begin(),
                                         allSystems.end(),
                                         [&system1](const System &system) {
                                           return system.dbId == system1.dbId;
                                         });
  EXPECT_NE(maybeSystem1, allSystems.end())
    << "Expected to find system " << system1.name << " among " << allSystems.size();

  const auto maybeSystem2 = std::find_if(allSystems.begin(),
                                         allSystems.end(),
                                         [&system2](const System &system) {
                                           return system.dbId == system2.dbId;
                                         });
  EXPECT_NE(maybeSystem2, allSystems.end())
    << "Expected to find system " << system2.name << " among " << allSystems.size();
}

TEST_F(Integration_Bsgalone_Server_Domain_Adapters_Driven_Repositories_SystemRepository,
       Save_PersistsSystemWhenItDoesNotExist)
{
  System system{
    .dbId        = core::Uuid{},
    .name        = std::format("random-system-{:%F%T}", ::core::now()),
    .position    = Eigen::Vector3f(2.5f, -1.2f, 3.4f),
    .currentTick = chrono::Tick::fromInt(34),
    .step        = chrono::TimeStep(15, chrono::Duration::fromSeconds(1.0f)),
  };

  SystemRepository repo(this->dbConnection());
  repo.initialize();
  repo.save(system);

  const auto actual = repo.findOneById(system.dbId);
  EXPECT_EQ(system.dbId, actual.dbId);
  EXPECT_EQ(system.name, actual.name);
  EXPECT_EQ(system.position, actual.position);
  EXPECT_EQ(system.currentTick, actual.currentTick);
  EXPECT_EQ(system.step, actual.step);
}

TEST_F(Integration_Bsgalone_Server_Domain_Adapters_Driven_Repositories_SystemRepository,
       Save_TruncatesTimeStepWhenNotInteger)
{
  System system{
    .dbId        = core::Uuid{},
    .name        = std::format("random-system-{:%F%T}", ::core::now()),
    .position    = Eigen::Vector3f(2.5f, -1.2f, 3.4f),
    .currentTick = chrono::Tick::fromInt(34),
    .step        = chrono::TimeStep(15, chrono::Duration::fromSeconds(1.1f)),
  };

  SystemRepository repo(this->dbConnection());
  repo.initialize();
  repo.save(system);

  const auto actual = repo.findOneById(system.dbId).step.data();
  EXPECT_EQ(1.0f, actual.duration.elapsed);
  EXPECT_EQ(chrono::Unit::SECONDS, actual.duration.unit);
}

TEST_F(Integration_Bsgalone_Server_Domain_Adapters_Driven_Repositories_SystemRepository,
       Save_UpdatesPositionWhenSystemAlreadyExists)
{
  auto system = insertTestSystem(*this->dbConnection(), true);

  // Note: the position is a numeric value with 2 decimal places. It
  // is not possible to put more digits in the below vector.
  const Eigen::Vector3f updatedPosition(-78.45f, 654.51f, 1247.03f);
  ASSERT_NE(updatedPosition, system.position);
  system.position = updatedPosition;
  SystemRepository repo(this->dbConnection());
  repo.initialize();
  repo.save(system);

  const auto actual = repo.findOneById(system.dbId);
  EXPECT_EQ(updatedPosition, actual.position);
}

TEST_F(Integration_Bsgalone_Server_Domain_Adapters_Driven_Repositories_SystemRepository,
       Save_UpdatesCurrentTickWhenSystemAlreadyExists)
{
  auto system = insertTestSystem(*this->dbConnection(), true);

  system.currentTick += chrono::TickDuration::fromInt(12);
  SystemRepository repo(this->dbConnection());
  repo.initialize();
  repo.save(system);

  const auto actual = repo.findOneById(system.dbId);
  EXPECT_EQ(system.currentTick, actual.currentTick);
}

TEST_F(Integration_Bsgalone_Server_Domain_Adapters_Driven_Repositories_SystemRepository,
       Save_UpdatesTickConfigWhenSystemAlreadyExists)
{
  auto system = insertTestSystem(*this->dbConnection(), true);

  system.step = chrono::TimeStep(27, chrono::Duration::fromSeconds(3.0f));
  SystemRepository repo(this->dbConnection());
  repo.initialize();
  repo.save(system);

  const auto actual = repo.findOneById(system.dbId);
  EXPECT_EQ(system.step, actual.step);
}

} // namespace bsgalone::server
