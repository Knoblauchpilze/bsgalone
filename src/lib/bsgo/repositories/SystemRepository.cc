
#include "SystemRepository.hh"

namespace bsgo {

SystemRepository::SystemRepository(const DbConnectionShPtr &connection)
  : AbstractRepository("system", connection)
{}

namespace {
constexpr auto SQL_QUERY_ALL = "SELECT id FROM system";

constexpr auto SQL_QUERY_SYSTEM = "SELECT name, x_pos, y_pos, z_pos FROM system WHERE id = ";
auto generateSystemSqlQuery(const Uuid &system) -> std::string
{
  return SQL_QUERY_SYSTEM + std::to_string(toDbId(system));
}

constexpr auto SQL_QUERY_ASTEROID = "SELECT id FROM asteroid WHERE system = ";
auto generateAsteroidSqlQuery(const Uuid &system) -> std::string
{
  return SQL_QUERY_ASTEROID + std::to_string(toDbId(system));
}

constexpr auto SQL_QUERY_SHIP = "SELECT ship FROM ship_system WHERE system = ";
auto generateShipSqlQuery(const Uuid &system) -> std::string
{
  return SQL_QUERY_SHIP + std::to_string(toDbId(system));
}

constexpr auto SQL_QUERY_OUTPOST = "SELECT id FROM system_outpost WHERE system = ";
auto generateOutpostSqlQuery(const Uuid &system) -> std::string
{
  return SQL_QUERY_OUTPOST + std::to_string(toDbId(system));
}
} // namespace

auto SystemRepository::findAll() const -> std::unordered_set<Uuid>
{
  pqxx::nontransaction work(m_connection->connection());
  const auto rows(work.exec(SQL_QUERY_ALL));

  std::unordered_set<Uuid> out;
  for (const auto record : rows)
  {
    out.emplace(fromDbId(record[0].as<int>()));
  }

  return out;
}

auto SystemRepository::findOneById(const Uuid &system) const -> System
{
  const auto sql = generateSystemSqlQuery(system);

  pqxx::nontransaction work(m_connection->connection());
  const auto rows(work.exec(sql));

  if (rows.size() != 1)
  {
    error("Expected to find only one system with id " + str(system));
  }

  System out{};

  const auto &record = rows[0];
  out.id             = system;
  out.name           = record[0].as<std::string>();

  const auto x = record[1].as<float>();
  const auto y = record[2].as<float>();
  const auto z = record[3].as<float>();
  out.position = Eigen::Vector3f(x, y, z);

  return out;
}

auto SystemRepository::findAllAsteroidsBySystem(const Uuid &system) const
  -> std::unordered_set<Uuid>
{
  const auto sql = generateAsteroidSqlQuery(system);

  pqxx::nontransaction work(m_connection->connection());
  const auto rows(work.exec(sql));

  std::unordered_set<Uuid> out;
  for (const auto record : rows)
  {
    out.emplace(fromDbId(record[0].as<int>()));
  }

  return out;
}

auto SystemRepository::findAllShipsBySystem(const Uuid &system) const -> std::unordered_set<Uuid>
{
  const auto sql = generateShipSqlQuery(system);

  pqxx::nontransaction work(m_connection->connection());
  const auto rows(work.exec(sql));

  std::unordered_set<Uuid> out;
  for (const auto record : rows)
  {
    out.emplace(fromDbId(record[0].as<int>()));
  }

  return out;
}

auto SystemRepository::findAllOutpostsBySystem(const Uuid &system) const -> std::unordered_set<Uuid>
{
  const auto sql = generateOutpostSqlQuery(system);

  pqxx::nontransaction work(m_connection->connection());
  const auto rows(work.exec(sql));

  std::unordered_set<Uuid> out;
  for (const auto record : rows)
  {
    out.emplace(fromDbId(record[0].as<int>()));
  }

  return out;
}

} // namespace bsgo
