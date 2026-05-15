
#include "AsteroidRepository.hh"
#include "DbConnectionFixture.hh"
#include "DbQueryHelper.hh"
#include <gmock/gmock-matchers.h>
#include <gtest/gtest.h>

using namespace test;
using namespace ::testing;

namespace bsgalone::server {
using Integration_Bsgalone_Server_Domain_Adapters_Driven_Repositories_AsteroidRepository
  = DbConnectionFixture;

namespace {
auto insertTestAsteroid(DbConnection &dbConnection, const core::Uuid systemDbId) -> core::Asteroid
{
  const bsgalone::core::Uuid uuid;

  constexpr auto QUERY = R"(
      INSERT INTO asteroid ("id", "system", "max_health", "health", "radius", "x_pos", "y_pos", "z_pos")
        VALUES ($1, $2, 147.0, 18.0, 7.41, 1.0, 2.87, -32.14)
    )";

  const auto query = [&uuid, &systemDbId](pqxx::nontransaction &work) {
    return work.exec(QUERY, pqxx::params{uuid.toDbId(), systemDbId.toDbId()}).no_rows();
  };
  dbConnection.executeQuery(query);

  return core::Asteroid{
    .dbId      = uuid,
    .position  = Eigen::Vector3f(1.0f, 2.87f, -32.14f),
    .radius    = 7.41f,
    .health    = 18.0,
    .maxHealth = 147.0,
  };
}

auto insertTestLoot(DbConnection &dbConnection, const core::Uuid asteroidDbId) -> core::Loot
{
  const bsgalone::core::Uuid uuid;

  // The resource is hardcoded to tylium
  constexpr auto QUERY = R"(
      INSERT INTO asteroid_loot ("asteroid", "resource", "amount")
        VALUES ($1, 'f9f7636d-5a23-453e-b934-840b8b3ce74b', 1569)
    )";

  const auto query = [&asteroidDbId](pqxx::nontransaction &work) {
    return work.exec(QUERY, pqxx::params{asteroidDbId.toDbId()}).no_rows();
  };
  dbConnection.executeQuery(query);

  return core::Loot{
    .resource = core::Uuid::fromDbId("f9f7636d-5a23-453e-b934-840b8b3ce74b"),
    .amount   = 1569,
  };
}
} // namespace

TEST_F(Integration_Bsgalone_Server_Domain_Adapters_Driven_Repositories_AsteroidRepository,
       InitializeDoesNotThrow)
{
  AsteroidRepository repo(this->dbConnection());

  EXPECT_NO_THROW([&repo]() { repo.initialize(); }());
}

TEST_F(Integration_Bsgalone_Server_Domain_Adapters_Driven_Repositories_AsteroidRepository,
       FailsWhenInitializeIsNotCalled)
{
  AsteroidRepository repo(this->dbConnection());

  EXPECT_THAT([&repo]() { repo.findAllBySystem(core::Uuid{}); },
              ThrowsMessage<::core::CoreException>("Failed to execute sql query"));
}

TEST_F(Integration_Bsgalone_Server_Domain_Adapters_Driven_Repositories_AsteroidRepository,
       FindAllBySystem_ReturnsEmptyListWhenNoAsteroidAreRegistered)
{
  AsteroidRepository repo(this->dbConnection());
  repo.initialize();

  const auto actual = repo.findAllBySystem(core::Uuid{});

  EXPECT_TRUE(actual.empty());
}

TEST_F(Integration_Bsgalone_Server_Domain_Adapters_Driven_Repositories_AsteroidRepository,
       FindAllBySystem_ReturnsAsteroidWhenSomeAreAvailable)
{
  auto system   = insertTestSystem(*this->dbConnection());
  auto asteroid = insertTestAsteroid(*this->dbConnection(), system.dbId);

  AsteroidRepository repo(this->dbConnection());
  repo.initialize();

  const auto actual = repo.findAllBySystem(system.dbId);

  EXPECT_EQ(1u, actual.size());
  EXPECT_EQ(asteroid.dbId, actual[0].dbId);
  EXPECT_EQ(asteroid.position, actual[0].position);
  EXPECT_EQ(asteroid.radius, actual[0].radius);
  EXPECT_EQ(asteroid.dbId, actual[0].dbId);
  EXPECT_EQ(asteroid.health, actual[0].health);
  EXPECT_EQ(asteroid.maxHealth, actual[0].maxHealth);
  EXPECT_EQ(asteroid.loot, actual[0].loot);
}

TEST_F(Integration_Bsgalone_Server_Domain_Adapters_Driven_Repositories_AsteroidRepository,
       FindAllBySystem_ReturnsOnlyAsteroidFromRequestedSystem)
{
  auto system1   = insertTestSystem(*this->dbConnection());
  auto asteroid1 = insertTestAsteroid(*this->dbConnection(), system1.dbId);

  auto system2 = insertTestSystem(*this->dbConnection());
  insertTestAsteroid(*this->dbConnection(), system2.dbId);

  AsteroidRepository repo(this->dbConnection());
  repo.initialize();

  const auto actual = repo.findAllBySystem(system1.dbId);

  EXPECT_EQ(1u, actual.size());
  EXPECT_EQ(asteroid1.dbId, actual[0].dbId);
}

TEST_F(Integration_Bsgalone_Server_Domain_Adapters_Driven_Repositories_AsteroidRepository,
       FindAllBySystem_ReturnsLootWhenAsteroidHasOne)
{
  auto system   = insertTestSystem(*this->dbConnection());
  auto asteroid = insertTestAsteroid(*this->dbConnection(), system.dbId);
  auto loot     = insertTestLoot(*this->dbConnection(), asteroid.dbId);

  AsteroidRepository repo(this->dbConnection());
  repo.initialize();

  const auto actual = repo.findAllBySystem(system.dbId);

  EXPECT_EQ(1u, actual.size());
  EXPECT_EQ(loot.resource, actual[0].loot->resource);
  EXPECT_EQ(loot.amount, actual[0].loot->amount);
}

} // namespace bsgalone::server
