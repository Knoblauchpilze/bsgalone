
#include "OutpostRepository.hh"

namespace bsgo {

OutpostRepository::OutpostRepository(const DbConnectionShPtr &connection)
  : AbstractRepository("outpost", connection)
{}

namespace {
constexpr auto SQL_QUERY
  = "SELECT faction, max_hull_points, hull_points_regen, max_power_points, power_points_regen, radius FROM outpost WHERE id = ";
auto generateSqlQuery(const Uuid &ship) -> std::string
{
  return SQL_QUERY + std::to_string(toDbId(ship));
}
} // namespace

auto OutpostRepository::findOneById(const Uuid &outpost) const -> Outpost
{
  const auto sql = generateSqlQuery(outpost);

  pqxx::nontransaction work(m_connection->connection());
  const auto rows(work.exec(sql));

  if (rows.size() != 1)
  {
    error("Expected to find only one outpost with id " + str(outpost));
  }

  Outpost out;

  const auto &record  = rows[0];
  out.faction         = fromDbFaction(record[0].as<std::string>());
  out.maxHullPoints   = record[1].as<float>();
  out.hullPointsRegen = record[2].as<float>();
  out.maxPowerPoints  = record[3].as<float>();
  out.powerRegen      = record[4].as<float>();
  out.radius          = record[5].as<float>();

  return out;
}

} // namespace bsgo
