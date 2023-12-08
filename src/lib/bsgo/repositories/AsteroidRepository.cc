
#include "AsteroidRepository.hh"

namespace bsgo {

AsteroidRepository::AsteroidRepository(const DbConnectionShPtr &connection)
  : AbstractRepository("asteroid", connection)
{}

namespace {
constexpr auto SQL_QUERY = "SELECT health, radius, x_pos, y_pos, z_pos FROM asteroid WHERE id = ";
auto generateSqlQuery(const Uuid &asteroid) -> std::string
{
  return SQL_QUERY + std::to_string(toDbId(asteroid));
}

constexpr auto SQL_QUERY_LOOT = "SELECT count(resource) FROM asteroid_loot WHERE asteroid = ";
auto generateLootSqlQuery(const Uuid &asteroid) -> std::string
{
  return SQL_QUERY_LOOT + std::to_string(toDbId(asteroid));
}
} // namespace

auto AsteroidRepository::findOneById(const Uuid &asteroid) const -> Asteroid
{
  auto out = fetchAsteroidBase(asteroid);
  fetchLoot(asteroid, out);

  return out;
}

auto AsteroidRepository::fetchAsteroidBase(const Uuid &asteroid) const -> Asteroid
{
  const auto sql = generateSqlQuery(asteroid);

  pqxx::nontransaction work(m_connection->connection());
  const auto rows(work.exec(sql));

  if (rows.size() != 1)
  {
    error("Expected to find only one asteroid with id " + str(asteroid));
  }

  Asteroid out;

  const auto &record = rows[0];
  out.health         = record[0].as<float>();
  out.radius         = record[1].as<float>();

  const auto x = record[2].as<float>();
  const auto y = record[3].as<float>();
  const auto z = record[4].as<float>();
  out.position = Eigen::Vector3f(x, y, z);

  return out;
}

void AsteroidRepository::fetchLoot(const Uuid &asteroid, Asteroid &out) const
{
  const auto sql = generateLootSqlQuery(asteroid);

  pqxx::nontransaction work(m_connection->connection());
  const auto rows(work.exec(sql));

  if (rows.size() != 1)
  {
    error("Expected to find only one loot for asteroid with id " + str(asteroid));
  }

  const auto &record = rows[0];
  out.loot           = record[0].as<int>() > 0;
}

} // namespace bsgo
