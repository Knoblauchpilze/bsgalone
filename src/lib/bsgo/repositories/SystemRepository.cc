
#include "SystemRepository.hh"

namespace bsgo {

SystemRepository::SystemRepository(const DbConnectionShPtr &connection)
  : AbstractRepository("system", connection)
{}

namespace {
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

auto SystemRepository::findAllAsteroidsBySystem(const Uuid &system) const
  -> std::unordered_set<Uuid>
{
  const auto sql = generateAsteroidSqlQuery(system);

  pqxx::nontransaction work(m_connection->connection());
  pqxx::result rows(work.exec(sql));

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
  pqxx::result rows(work.exec(sql));

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
  pqxx::result rows(work.exec(sql));

  std::unordered_set<Uuid> out;
  for (const auto record : rows)
  {
    out.emplace(fromDbId(record[0].as<int>()));
  }

  return out;
}

} // namespace bsgo
