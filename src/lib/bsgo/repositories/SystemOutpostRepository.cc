
#include "SystemOutpostRepository.hh"

namespace bsgo {

SystemOutpostRepository::SystemOutpostRepository(const DbConnectionShPtr &connection)
  : AbstractRepository("system", connection)
{
  addModule("outpost");
}

namespace {
constexpr auto SQL_QUERY
  = "SELECT o.faction, so.hull_points, o.max_hull_points, o.hull_points_regen, so.power_points, o.max_power_points, o.power_points_regen, o.radius, so.x_pos, so.y_pos, so.z_pos FROM system_outpost AS so LEFT JOIN outpost AS o ON so.outpost = o.id WHERE so.id = ";
auto generateSqlQuery(const Uuid &outpost) -> std::string
{
  return SQL_QUERY + std::to_string(toDbId(outpost));
}

} // namespace

auto SystemOutpostRepository::findOneById(const Uuid &outpost) const -> SystemOutpost
{
  const auto sql = generateSqlQuery(outpost);

  pqxx::nontransaction work(m_connection->connection());
  pqxx::result rows(work.exec(sql));

  if (rows.size() != 1)
  {
    error("Expected to find only one system outpost with id " + str(outpost));
  }

  SystemOutpost out;

  const auto &record  = rows[0];
  out.faction         = fromDbFaction(record[0].as<std::string>());
  out.hullPoints      = record[1].as<float>();
  out.maxHullPoints   = record[2].as<float>();
  out.hullPointsRegen = record[3].as<float>();
  out.powerPoints     = record[4].as<float>();
  out.maxPowerPoints  = record[5].as<float>();
  out.powerRegen      = record[6].as<float>();
  out.radius          = record[7].as<float>();

  const auto x = record[8].as<float>();
  const auto y = record[9].as<float>();
  const auto z = record[10].as<float>();
  out.position = Eigen::Vector3f(x, y, z);

  return out;
}

} // namespace bsgo
