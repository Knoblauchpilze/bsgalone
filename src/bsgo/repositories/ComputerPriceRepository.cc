
#include "ComputerPriceRepository.hh"

namespace bsgo {

ComputerPriceRepository::ComputerPriceRepository(const DbConnectionShPtr &connection)
  : AbstractRepository("computer", connection)
{
  addModule("price");
}

namespace {
constexpr auto FIND_ALL_QUERY_NAME = "computer_price_find_all";
constexpr auto FIND_ALL_QUERY = "SELECT resource, cost FROM computer_price WHERE computer = $1";
} // namespace

void ComputerPriceRepository::initialize()
{
  m_connection->prepare(FIND_ALL_QUERY_NAME, FIND_ALL_QUERY);
}

auto ComputerPriceRepository::findAllByComputer(const Uuid computer) const
  -> std::unordered_map<Uuid, float>
{
  const auto query = [computer](pqxx::nontransaction &work) {
    return work.exec_prepared(FIND_ALL_QUERY_NAME, toDbId(computer));
  };
  const auto rows = m_connection->executeQuery(query);

  std::unordered_map<Uuid, float> out;
  for (const auto record : rows)
  {
    const auto resource = fromDbId(record[0].as<float>());
    const auto cost     = record[1].as<float>();

    out[resource] = cost;
  }

  return out;
}

} // namespace bsgo
