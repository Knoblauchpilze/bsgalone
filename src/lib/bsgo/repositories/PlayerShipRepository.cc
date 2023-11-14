
#include "PlayerShipRepository.hh"

namespace bsgo {

PlayerShipRepository::PlayerShipRepository(const DbConnectionShPtr &connection)
  : AbstractRepository("player", connection)
{
  addModule("ship");
}

namespace {
constexpr auto SQL_QUERY
  = "SELECT s.faction, s.name, ps.player, ps.hull_points, s.max_hull_points, s.hull_points_regen, ps.power_points, s.max_power_points, s.power_points_regen, s.max_acceleration, s.max_speed, s.radius, ps.x_pos, ps.y_pos, ps.z_pos FROM player_ship AS ps LEFT JOIN ship AS s ON ps.ship = s.id WHERE ps.id = ";
auto generateSqlQuery(const Uuid &ship) -> std::string
{
  return SQL_QUERY + std::to_string(toDbId(ship));
}
} // namespace

auto PlayerShipRepository::findOneById(const Uuid &ship) const -> PlayerShip
{
  const auto sql = generateSqlQuery(ship);

  pqxx::nontransaction work(m_connection->connection());
  pqxx::result rows(work.exec(sql));

  if (rows.size() != 1)
  {
    error("Expected to find only one player ship with id " + str(ship));
  }

  PlayerShip out;

  const auto &record = rows[0];
  out.faction        = fromDbFaction(record[0].as<std::string>());
  out.name           = record[1].as<std::string>();
  if (!record[2].is_null())
  {
    out.player = {fromDbId(record[2].as<int>())};
  }
  out.hullPoints      = record[3].as<float>();
  out.maxHullPoints   = record[4].as<float>();
  out.hullPointsRegen = record[5].as<float>();
  out.powerPoints     = record[6].as<float>();
  out.maxPowerPoints  = record[7].as<float>();
  out.powerRegen      = record[8].as<float>();
  out.acceleration    = record[9].as<float>();
  out.speed           = record[10].as<float>();
  out.radius          = record[11].as<float>();

  const auto x = record[12].as<float>();
  const auto y = record[13].as<float>();
  const auto z = record[14].as<float>();
  out.position = Eigen::Vector3f(x, y, z);

  return out;
}

auto PlayerShipRepository::findAllByPlayer(const Uuid &player) const -> std::vector<Uuid>
{
  if (player != Uuid{0})
  {
    error("Player " + str(player) + " not found");
  }
  return {Uuid{0}, Uuid{2}};
}

} // namespace bsgo
