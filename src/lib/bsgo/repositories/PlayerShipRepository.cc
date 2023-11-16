
#include "PlayerShipRepository.hh"

namespace bsgo {

PlayerShipRepository::PlayerShipRepository(const DbConnectionShPtr &connection)
  : AbstractRepository("player", connection)
{
  addModule("ship");
}

namespace {
constexpr auto SQL_QUERY
  = "SELECT s.faction, s.name, ps.player, ps.active, ps.hull_points, s.max_hull_points, s.hull_points_regen, ps.power_points, s.max_power_points, s.power_points_regen, s.max_acceleration, s.max_speed, s.radius, ps.x_pos, ps.y_pos, ps.z_pos FROM player_ship AS ps LEFT JOIN ship AS s ON ps.ship = s.id WHERE ps.id = ";
auto generateSqlQuery(const Uuid &ship) -> std::string
{
  return SQL_QUERY + std::to_string(toDbId(ship));
}

constexpr auto SQL_QUERY_SLOT
  = "SELECT ss.type, COUNT(ss.id) FROM player_ship AS ps LEFT JOIN ship AS s ON ps.ship = s.id LEFT JOIN ship_slot AS ss ON s.id = ss.ship WHERE ps.id = ";
constexpr auto SQL_QUERY_SLOT_GROUP = " GROUP BY ss.type";
auto generateSlotSqlQuery(const Uuid &ship) -> std::string
{
  return SQL_QUERY_SLOT + std::to_string(toDbId(ship)) + SQL_QUERY_SLOT_GROUP;
}

constexpr auto SQL_QUERY_ALL = "SELECT id FROM player_ship WHERE player = ";
auto generateAllSqlQuery(const Uuid &player) -> std::string
{
  return SQL_QUERY_ALL + std::to_string(toDbId(player));
}
} // namespace

auto PlayerShipRepository::findOneById(const Uuid &ship) const -> PlayerShip
{
  auto out = fetchShipBase(ship);
  fetchSlots(ship, out);

  return out;
}

auto PlayerShipRepository::findAllByPlayer(const Uuid &player) const -> std::unordered_set<Uuid>
{
  const auto sql = generateAllSqlQuery(player);

  pqxx::nontransaction work(m_connection->connection());
  pqxx::result rows(work.exec(sql));

  std::unordered_set<Uuid> out;
  for (const auto record : rows)
  {
    out.emplace(fromDbId(record[0].as<int>()));
  }

  return out;
}

auto PlayerShipRepository::fetchShipBase(const Uuid &ship) const -> PlayerShip
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
  out.active          = record[3].as<bool>();
  out.hullPoints      = record[4].as<float>();
  out.maxHullPoints   = record[5].as<float>();
  out.hullPointsRegen = record[6].as<float>();
  out.powerPoints     = record[7].as<float>();
  out.maxPowerPoints  = record[8].as<float>();
  out.powerRegen      = record[9].as<float>();
  out.acceleration    = record[10].as<float>();
  out.speed           = record[11].as<float>();
  out.radius          = record[12].as<float>();

  const auto x = record[13].as<float>();
  const auto y = record[14].as<float>();
  const auto z = record[15].as<float>();
  out.position = Eigen::Vector3f(x, y, z);

  return out;
}

void PlayerShipRepository::fetchSlots(const Uuid &ship, PlayerShip &out) const
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
