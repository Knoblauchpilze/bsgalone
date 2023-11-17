
#include "ShipRepository.hh"

namespace bsgo {

ShipRepository::ShipRepository(const DbConnectionShPtr &connection)
  : AbstractRepository("ship", connection)
{}

namespace {
constexpr auto SQL_QUERY
  = "SELECT faction, class, name, max_hull_points, hull_points_regen, max_power_points, power_points_regen, max_acceleration, max_speed, radius FROM ship WHERE id = ";
auto generateSqlQuery(const Uuid &ship) -> std::string
{
  return SQL_QUERY + std::to_string(toDbId(ship));
}

constexpr auto SQL_QUERY_SLOT       = "SELECT type, COUNT(id) FROM ship_slot WHERE ship = ";
constexpr auto SQL_QUERY_SLOT_GROUP = " GROUP BY type";
auto generateSlotSqlQuery(const Uuid &ship) -> std::string
{
  return SQL_QUERY_SLOT + std::to_string(toDbId(ship)) + SQL_QUERY_SLOT_GROUP;
}
} // namespace

auto ShipRepository::findOneById(const Uuid &ship) const -> Ship
{
  auto out = fetchShipBase(ship);
  fetchSlots(ship, out);

  return out;
}

auto ShipRepository::fetchShipBase(const Uuid &ship) const -> Ship
{
  const auto sql = generateSqlQuery(ship);

  pqxx::nontransaction work(m_connection->connection());
  pqxx::result rows(work.exec(sql));

  if (rows.size() != 1)
  {
    error("Expected to find only one ship with id " + str(ship));
  }

  Ship out;

  const auto &record  = rows[0];
  out.faction         = fromDbFaction(record[0].as<std::string>());
  out.shipClass       = fromDbShipClass(record[1].as<std::string>());
  out.name            = record[2].as<std::string>();
  out.maxHullPoints   = record[3].as<float>();
  out.hullPointsRegen = record[4].as<float>();
  out.maxPowerPoints  = record[5].as<float>();
  out.powerRegen      = record[6].as<float>();
  out.acceleration    = record[7].as<float>();
  out.speed           = record[8].as<float>();
  out.radius          = record[9].as<float>();

  return out;
}

void ShipRepository::fetchSlots(const Uuid &ship, Ship &out) const
{
  const auto sql = generateSlotSqlQuery(ship);

  pqxx::nontransaction work(m_connection->connection());
  pqxx::result rows(work.exec(sql));

  for (const auto record : rows)
  {
    const auto slot  = fromDbSlot(record[0].as<std::string>());
    const auto count = record[1].as<int>();

    out.slots[slot] = count;
  }
}

} // namespace bsgo
