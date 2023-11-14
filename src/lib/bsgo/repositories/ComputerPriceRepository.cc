
#include "ComputerPriceRepository.hh"

namespace bsgo {

ComputerPriceRepository::ComputerPriceRepository(const DbConnectionShPtr &connection)
  : AbstractRepository("computer", connection)
{
  addModule("price");
}

namespace {
constexpr auto SQL_QUERY = "SELECT resource, cost FROM computer_price WHERE computer = ";
auto generateSqlQuery(const Uuid &computer) -> std::string
{
  return SQL_QUERY + std::to_string(toDbId(computer));
}
} // namespace

auto ComputerPriceRepository::findAllByComputer(const Uuid &computer) const
  -> std::unordered_map<Uuid, int>
{
  const auto sql = generateSqlQuery(computer);

  pqxx::nontransaction work(m_connection->connection());
  pqxx::result rows(work.exec(sql));

  std::unordered_map<Uuid, int> out;
  for (const auto record : rows)
  {
    const auto resource = fromDbId(record[0].as<int>());
    const auto cost     = record[1].as<float>();

    out[resource] = cost;
  }

  return out;
}

} // namespace bsgo
