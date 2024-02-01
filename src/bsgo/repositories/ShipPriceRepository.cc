
#include "ShipPriceRepository.hh"

namespace bsgo {

ShipPriceRepository::ShipPriceRepository(const DbConnectionShPtr &connection)
  : AbstractRepository("ship", connection)
{
  addModule("price");
}

namespace {
constexpr auto FIND_ALL_QUERY_NAME = "ship_price_find_all";
constexpr auto FIND_ALL_QUERY      = "SELECT resource, cost FROM ship_price WHERE ship = $1";
} // namespace

void ShipPriceRepository::initialize()
{
  m_connection->prepare(FIND_ALL_QUERY_NAME, FIND_ALL_QUERY);
}

auto ShipPriceRepository::findAllByShip(const Uuid ship) const -> std::unordered_map<Uuid, float>
{
  auto work       = m_connection->nonTransaction();
  const auto rows = work.exec_prepared(FIND_ALL_QUERY_NAME, toDbId(ship));

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
