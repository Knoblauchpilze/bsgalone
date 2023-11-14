
#include "ShipComputerRepository.hh"

namespace bsgo {

ShipComputerRepository::ShipComputerRepository(const DbConnectionShPtr &connection)
  : AbstractRepository("computer", connection)
{
  addModule("ship");
}

namespace {
constexpr auto SQL_QUERY = "SELECT computer FROM ship_computer WHERE ship = ";
auto generateSqlQuery(const Uuid &ship) -> std::string
{
  return SQL_QUERY + std::to_string(toDbId(ship));
}
} // namespace

auto ShipComputerRepository::findAllByShip(const Uuid &ship) const -> std::unordered_set<Uuid>
{
  const auto sql = generateSqlQuery(ship);

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
