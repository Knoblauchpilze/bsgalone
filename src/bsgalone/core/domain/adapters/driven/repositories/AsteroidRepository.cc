
#include "AsteroidRepository.hh"

namespace bsgalone::core {

AsteroidRepository::AsteroidRepository(DbConnectionShPtr connection)
  : AbstractRepository("account", std::move(connection))
{}

namespace {
constexpr auto FIND_ALL_BY_SYSTEM_QUERY_NAME = "asteroid_find_all_by_system";
constexpr auto FIND_ALL_BY_SYSTEM_QUERY      = R"(
SELECT
  a.id,
  a.system,
  a.health,
  a.max_health,
  a.radius,
  a.x_pos,
  a.y_pos,
  a.z_pos
FROM
  asteroid AS a
  LEFT JOIN asteroid_respawn AS ar ON ar.asteroid = a.id
WHERE
  ar.died_at is null
  AND a.system = $1
)";

constexpr auto FIND_LOOT_QUERY_NAME = "asteroid_find_loot";
constexpr auto FIND_LOOT_QUERY = "SELECT resource, amount FROM asteroid_loot WHERE asteroid = $1";
} // namespace

void AsteroidRepository::initialize()
{
  m_connection->prepare(FIND_ALL_BY_SYSTEM_QUERY_NAME, FIND_ALL_BY_SYSTEM_QUERY);
  m_connection->prepare(FIND_LOOT_QUERY_NAME, FIND_LOOT_QUERY);
}

namespace {
auto fromDbRow(const pqxx::row &record) -> Asteroid
{
  const auto x = record[5].as<float>();
  const auto y = record[6].as<float>();
  const auto z = record[7].as<float>();

  return Asteroid{
    .dbId       = core::Uuid::fromDbId(record[0].view()),
    .systemDbId = core::Uuid::fromDbId(record[1].view()),
    .position   = Eigen::Vector3f(x, y, z),
    .radius     = record[4].as<float>(),
    .health     = record[2].as<float>(),
    .maxHealth  = record[3].as<float>(),
  };
}
} // namespace

auto AsteroidRepository::findAllBySystem(const Uuid systemDbId) const -> std::vector<Asteroid>
{
  const auto query = [systemDbId](pqxx::nontransaction &work) {
    return work.exec(pqxx::prepped{FIND_ALL_BY_SYSTEM_QUERY_NAME},
                     pqxx::params{systemDbId.toDbId()});
  };
  const auto rows = m_connection->executeQuery(query);

  std::vector<Asteroid> out;
  for (const auto record : rows)
  {
    Asteroid asteroid = fromDbRow(record);
    asteroid.loot     = fetchLoot(asteroid.dbId);
    out.emplace_back(std::move(asteroid));
  }

  return out;
}

auto AsteroidRepository::fetchLoot(const Uuid asteroidDbId) const -> std::optional<Loot>
{
  const auto query = [asteroidDbId](pqxx::nontransaction &work) {
    return work.exec(pqxx::prepped{FIND_LOOT_QUERY_NAME}, pqxx::params{asteroidDbId.toDbId()});
  };
  const auto rows = m_connection->executeQuery(query);

  if (rows.empty())
  {
    return {};
  }

  if (rows.size() != 1)
  {
    error("Expected to find only one loot for asteroid " + asteroidDbId.str());
  }

  const auto &record = rows[0];
  return Loot{
    .resource = core::Uuid::fromDbId(record[0].view()),
    .amount   = record[1].as<int>(),
  };
}

} // namespace bsgalone::core
