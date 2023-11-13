
#include "AsteroidLootRepository.hh"

namespace bsgo {

AsteroidLootRepository::AsteroidLootRepository(const DbConnectionShPtr &connection)
  : AbstractRepository("loot", connection)
{
  addModule("asteroid");
}

namespace {
constexpr auto SQL_QUERY_ASTEROID_LOOT
  = "SELECT resource, amount FROM asteroid_loot WHERE asteroid = ";

auto generateSqlQuery(const Uuid &asteroid) -> std::string
{
  return SQL_QUERY_ASTEROID_LOOT + str(asteroid + 1);
}
} // namespace

auto AsteroidLootRepository::findOneById(const Uuid &asteroid) const -> AsteroidLoot
{
  const auto sql = generateSqlQuery(asteroid);

  pqxx::nontransaction work(m_connection->connection());
  pqxx::result rows(work.exec(sql));

  if (1u != rows.size())
  {
    error("Expected to find loot for asteroid " + str(asteroid));
  }

  AsteroidLoot out;

  const auto &record = rows[0];
  out.resource       = record[0].as<Uuid>() - 1u;
  out.amount         = record[1].as<float>();

  return out;
}

} // namespace bsgo
