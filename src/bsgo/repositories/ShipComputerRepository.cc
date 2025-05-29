
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

constexpr auto FIND_ONE_BY_COMPUTER_QUERY_NAME = "ship_computer_find_one_by_computer";
constexpr auto FIND_ONE_BY_COMPUTER_QUERY
  = "SELECT COUNT(ship) FROM ship_computer WHERE computer = $1";

constexpr auto UPDATE_COMPUTER_QUERY_NAME = "ship_computer_update";
constexpr auto UPDATE_COMPUTER_QUERY      = R"(
INSERT INTO ship_computer (ship, computer)
  VALUES ($1, $2)
)";

constexpr auto DELETE_COMPUTER_QUERY_NAME = "ship_computer_delete";
constexpr auto DELETE_COMPUTER_QUERY = "DELETE FROM ship_computer WHERE ship = $1 AND computer = $2";
} // namespace

void ShipComputerRepository::initialize()
{
  m_connection->prepare(FIND_ALL_QUERY_NAME, FIND_ALL_QUERY);
  m_connection->prepare(FIND_ONE_QUERY_NAME, FIND_ONE_QUERY);
  m_connection->prepare(FIND_ONE_BY_COMPUTER_QUERY_NAME, FIND_ONE_BY_COMPUTER_QUERY);
  m_connection->prepare(UPDATE_COMPUTER_QUERY_NAME, UPDATE_COMPUTER_QUERY);
  m_connection->prepare(DELETE_COMPUTER_QUERY_NAME, DELETE_COMPUTER_QUERY);
}

bool ShipComputerRepository::existByShipAndComputer(const Uuid ship, const Uuid computer) const
{
  const auto query = [ship, computer](pqxx::nontransaction &work) {
    return work.exec(FIND_ONE_QUERY_NAME, pqxx::params{toDbId(ship), toDbId(computer)}).one_row();
  };
  const auto record = m_connection->executeQueryReturningSingleRow(query);

  return record[0].as<int>() > 0;
}

bool ShipComputerRepository::existByComputer(const Uuid computer) const
{
  const auto query = [computer](pqxx::nontransaction &work) {
    return work.exec(FIND_ONE_BY_COMPUTER_QUERY_NAME, pqxx::params{toDbId(computer)}).one_row();
  };
  const auto record = m_connection->executeQueryReturningSingleRow(query);

  return record[0].as<int>() > 0;
}

auto ShipComputerRepository::findAllByShip(const Uuid ship) const -> std::unordered_set<Uuid>
{
  const auto query = [ship](pqxx::nontransaction &work) {
    return work.exec(FIND_ALL_QUERY_NAME, pqxx::params{toDbId(ship)});
  };
  const auto rows = m_connection->executeQuery(query);

  std::unordered_set<Uuid> out;
  for (const auto record : rows)
  {
    out.emplace(fromDbId(record[0].as<int>()));
  }

  return out;
}

void ShipComputerRepository::save(const Uuid ship, const Uuid computer)
{
  auto query = [&ship, &computer](pqxx::work &transaction) {
    return transaction
      .exec(UPDATE_COMPUTER_QUERY_NAME, pqxx::params{toDbId(ship), toDbId(computer)})
      .no_rows();
  };

  const auto res = m_connection->tryExecuteTransaction(query);
  if (res.error)
  {
    error("Failed to save ship computer: " + *res.error);
  }
}

void ShipComputerRepository::deleteByShipAndId(const Uuid ship, const Uuid computer)
{
  auto query = [&ship, &computer](pqxx::work &transaction) {
    return transaction
      .exec(DELETE_COMPUTER_QUERY_NAME, pqxx::params{toDbId(ship), toDbId(computer)})
      .no_rows();
  };

  const auto res = m_connection->tryExecuteTransaction(query);
  if (res.error)
  {
    error("Failed to delete ship computer: " + *res.error);
  }
}

} // namespace bsgo
