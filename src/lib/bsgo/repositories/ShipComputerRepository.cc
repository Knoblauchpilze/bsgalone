
#include "ShipComputerRepository.hh"

namespace bsgo {

ShipComputerRepository::ShipComputerRepository(const DbConnectionShPtr &connection)
  : AbstractRepository("computer", connection)
{
  addModule("ship");
}

namespace {
constexpr auto FIND_ALL_QUERY_NAME = "ship_computer_find_all";
constexpr auto FIND_ALL_QUERY      = "SELECT computer FROM ship_computer WHERE ship = $1";

constexpr auto FIND_ONE_QUERY_NAME = "ship_computer_find_one";
constexpr auto FIND_ONE_QUERY
  = "SELECT COUNT(ship) FROM ship_computer WHERE ship = $1 AND computer = $2";

} // namespace

void ShipComputerRepository::initialize()
{
  m_connection->prepare(FIND_ALL_QUERY_NAME, FIND_ALL_QUERY);
  m_connection->prepare(FIND_ONE_QUERY_NAME, FIND_ONE_QUERY);
}

bool ShipComputerRepository::existByShipAndComputer(const Uuid &ship, const Uuid &computer) const
{
  auto work         = m_connection->nonTransaction();
  const auto record = work.exec_prepared1(FIND_ONE_QUERY_NAME, toDbId(ship), toDbId(computer));

  return record[0].as<int>() > 0;
}

auto ShipComputerRepository::findAllByShip(const Uuid &ship) const -> std::unordered_set<Uuid>
{
  auto work       = m_connection->nonTransaction();
  const auto rows = work.exec_prepared(FIND_ALL_QUERY_NAME, toDbId(ship));

  std::unordered_set<Uuid> out;
  for (const auto record : rows)
  {
    out.emplace(fromDbId(record[0].as<int>()));
  }

  return out;
}

} // namespace bsgo
