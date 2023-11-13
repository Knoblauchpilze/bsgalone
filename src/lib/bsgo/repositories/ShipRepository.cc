
#include "ShipRepository.hh"

namespace bsgo {

ShipRepository::ShipRepository(const DbConnectionShPtr &connection)
  : AbstractRepository("ship", connection)
{}

namespace {
constexpr auto SQL_QUERY
  = "SELECT faction, name, max_hull_points, hull_points_regen, max_power_points, power_points_regen, max_acceleration, max_speed, radius FROM ship WHERE id = ";

auto generateSqlQuery(const Uuid &ship) -> std::string
{
  return SQL_QUERY + std::to_string(toDbId(ship));
}
} // namespace

auto ShipRepository::findOneById(const Uuid &ship) const -> Ship
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
  out.name            = record[1].as<std::string>();
  out.maxHullPoints   = record[2].as<float>();
  out.hullPointsRegen = record[3].as<float>();
  out.maxPowerPoints  = record[4].as<float>();
  out.powerRegen      = record[5].as<float>();
  out.acceleration    = record[6].as<float>();
  out.speed           = record[7].as<float>();
  out.radius          = record[8].as<float>();

  return out;
}

} // namespace bsgo
