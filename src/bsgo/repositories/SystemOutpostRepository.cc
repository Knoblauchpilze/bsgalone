
#include "SystemOutpostRepository.hh"

namespace bsgo {

SystemOutpostRepository::SystemOutpostRepository(const DbConnectionShPtr &connection)
  : AbstractRepository("system", connection)
{
  addModule("outpost");
}

namespace {
constexpr auto FIND_ONE_QUERY_NAME = "system_outpost_find_one";
constexpr auto FIND_ONE_QUERY
  = "SELECT o.faction, so.hull_points, o.max_hull_points, o.hull_points_regen, so.power_points, o.max_power_points, o.power_points_regen, o.radius, so.x_pos, so.y_pos, so.z_pos FROM system_outpost AS so LEFT JOIN outpost AS o ON so.outpost = o.id WHERE so.id = $1";
} // namespace

void SystemOutpostRepository::initialize()
{
  m_connection->prepare(FIND_ONE_QUERY_NAME, FIND_ONE_QUERY);
}

auto SystemOutpostRepository::findOneById(const Uuid outpost) const -> SystemOutpost
{
  auto work         = m_connection->nonTransaction();
  const auto record = work.exec_prepared1(FIND_ONE_QUERY_NAME, toDbId(outpost));

  SystemOutpost out;

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
