
#include "AsteroidRepository.hh"

namespace bsgo {

AsteroidRepository::AsteroidRepository(const DbConnectionShPtr &connection)
  : AbstractRepository("asteroid", connection)
{}

namespace {
constexpr auto FIND_ONE_QUERY_NAME = "asteroid_find_one";
constexpr auto FIND_ONE_QUERY
  = "SELECT system, health, max_health, radius, x_pos, y_pos, z_pos FROM asteroid WHERE id = $1";

constexpr auto FIND_LOOT_QUERY_NAME = "asteroid_find_loot";
constexpr auto FIND_LOOT_QUERY = "SELECT count(resource) FROM asteroid_loot WHERE asteroid = $1";

constexpr auto FIND_ALL_BY_RESPAWN_TIME_QUERY_NAME = "asteroid_find_all_by_respawn";
constexpr auto FIND_ALL_BY_RESPAWN_TIME_QUERY      = R"(
SELECT
  ar.asteroid
FROM
  asteroid_respawn AS ar
  LEFT JOIN asteroid AS a ON a.id = ar.asteroid
WHERE
  a.system = $1
  AND ar.respawn_at <= $2;
)";

constexpr auto UPDATE_ASTEROID_QUERY_NAME = "asteroid_update";
constexpr auto UPDATE_ASTEROID_QUERY      = R"(
UPDATE asteroid
  SET
    health = $1
  WHERE
    id = $2
)";

constexpr auto UPDATE_RESPAWN_TIME_QUERY_NAME = "asteroid_update_respawn";
constexpr auto UPDATE_RESPAWN_TIME_QUERY      = R"(
INSERT INTO asteroid_respawn ("asteroid", "died_at", "respawn_at")
  VALUES($1, $2, $3)
)";

constexpr auto DELETE_RESPAWN_QUERY_NAME = "asteroid_delete_respawn";
constexpr auto DELETE_RESPAWN_QUERY      = R"(
DELETE FROM
  asteroid_respawn
WHERE
  asteroid = $1
)";
} // namespace

void AsteroidRepository::initialize()
{
  m_connection->prepare(FIND_ONE_QUERY_NAME, FIND_ONE_QUERY);
  m_connection->prepare(FIND_LOOT_QUERY_NAME, FIND_LOOT_QUERY);
  m_connection->prepare(FIND_ALL_BY_RESPAWN_TIME_QUERY_NAME, FIND_ALL_BY_RESPAWN_TIME_QUERY);
  m_connection->prepare(UPDATE_ASTEROID_QUERY_NAME, UPDATE_ASTEROID_QUERY);
  m_connection->prepare(UPDATE_RESPAWN_TIME_QUERY_NAME, UPDATE_RESPAWN_TIME_QUERY);
  m_connection->prepare(DELETE_RESPAWN_QUERY_NAME, DELETE_RESPAWN_QUERY);
}

auto AsteroidRepository::findOneById(const Uuid asteroid) const -> Asteroid
{
  auto out = fetchAsteroidBase(asteroid);
  fetchLoot(asteroid, out);

  return out;
}

auto AsteroidRepository::findAllBySystemAndRespawnTime(const Uuid systemDbId,
                                                       const chrono::Tick &until)
  -> std::vector<Asteroid>
{
  const auto query = [&systemDbId, &until](pqxx::nontransaction &work) {
    return work.exec(pqxx::prepped{FIND_ALL_BY_RESPAWN_TIME_QUERY_NAME},
                     pqxx::params{toDbId(systemDbId), until.count()});
  };
  const auto rows = m_connection->executeQuery(query);

  std::vector<Asteroid> out;
  for (const auto record : rows)
  {
    const auto dbId = fromDbId(record[0].as<int>());
    out.emplace_back(findOneById(dbId));
  }

  return out;
}

void AsteroidRepository::save(const Asteroid &asteroid)
{
  auto query = [&asteroid](pqxx::work &transaction) {
    return transaction
      .exec(pqxx::prepped{UPDATE_ASTEROID_QUERY_NAME},
            pqxx::params{asteroid.health, toDbId(asteroid.id)})
      .no_rows();
  };

  const auto res = m_connection->tryExecuteTransaction(query);
  if (res.error)
  {
    error("Failed to save asteroid: " + *res.error);
  }
}

void AsteroidRepository::saveRespawn(const Uuid asteroid,
                                     const chrono::Tick &death,
                                     const chrono::Tick &respawn)
{
  auto query = [&asteroid, &death, &respawn](pqxx::work &transaction) {
    return transaction
      .exec(pqxx::prepped{UPDATE_RESPAWN_TIME_QUERY_NAME},
            pqxx::params{toDbId(asteroid), death.count(), respawn.count()})
      .no_rows();
  };

  const auto res = m_connection->tryExecuteTransaction(query);
  if (res.error)
  {
    error("Failed to save asteroid respawn: " + *res.error);
  }
}

void AsteroidRepository::deleteRespawn(const Uuid asteroid)
{
  auto query = [&asteroid](pqxx::work &transaction) {
    return transaction
      .exec(pqxx::prepped{DELETE_RESPAWN_QUERY_NAME}, pqxx::params{toDbId(asteroid)})
      .no_rows();
  };

  const auto res = m_connection->tryExecuteTransaction(query);
  if (res.error)
  {
    error("Failed to delete asteroid respawn: " + *res.error);
  }
}

auto AsteroidRepository::fetchAsteroidBase(const Uuid asteroid) const -> Asteroid
{
  const auto query = [asteroid](pqxx::nontransaction &work) {
    return work.exec(pqxx::prepped{FIND_ONE_QUERY_NAME}, pqxx::params{toDbId(asteroid)}).one_row();
  };
  const auto record = m_connection->executeQueryReturningSingleRow(query);

  Asteroid out;

  out.id        = asteroid;
  out.system    = fromDbId(record[0].as<int>());
  out.health    = record[1].as<float>();
  out.maxHealth = record[2].as<float>();
  out.radius    = record[3].as<float>();

  const auto x = record[4].as<float>();
  const auto y = record[5].as<float>();
  const auto z = record[6].as<float>();
  out.position = Eigen::Vector3f(x, y, z);

  return out;
}

void AsteroidRepository::fetchLoot(const Uuid asteroid, Asteroid &out) const
{
  const auto query = [asteroid](pqxx::nontransaction &work) {
    return work.exec(pqxx::prepped{FIND_LOOT_QUERY_NAME}, pqxx::params{toDbId(asteroid)}).one_row();
  };
  const auto record = m_connection->executeQueryReturningSingleRow(query);

  out.loot = record[0].as<int>() > 0;
}

} // namespace bsgo
