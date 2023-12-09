
#include "PlayerShipRepository.hh"

namespace bsgo {

PlayerShipRepository::PlayerShipRepository(const DbConnectionShPtr &connection)
  : AbstractRepository("player", connection)
{
  addModule("ship");
}

namespace {
constexpr auto FIND_ALL_QUERY_NAME = "player_ship_find_all";
constexpr auto FIND_ALL_QUERY      = "SELECT id FROM player_ship WHERE player = $1";

constexpr auto FIND_ONE_QUERY_NAME = "player_ship_find_one";
constexpr auto FIND_ONE_QUERY
  = "SELECT s.faction, s.class, s.name, ps.player, ps.active, ps.hull_points, s.max_hull_points, s.hull_points_regen, ps.power_points, s.max_power_points, s.power_points_regen, s.max_acceleration, s.max_speed, s.radius, ps.x_pos, ps.y_pos, ps.z_pos, sc.jump_time_ms, sc.jump_time_threat_ms FROM player_ship AS ps LEFT JOIN ship AS s ON ps.ship = s.id LEFT JOIN ship_class AS sc ON s.class = sc.name WHERE ps.id = $1";

constexpr auto FIND_SLOTS_QUERY_NAME = "player_ship_find_slots";
constexpr auto FIND_SLOTS_QUERY
  = "SELECT ss.type, COUNT(ss.id) FROM player_ship AS ps LEFT JOIN ship AS s ON ps.ship = s.id LEFT JOIN ship_slot AS ss ON s.id = ss.ship WHERE ps.id = $1 GROUP BY ss.type";
} // namespace

void PlayerShipRepository::initialize()
{
  m_connection->prepare(FIND_ALL_QUERY_NAME, FIND_ALL_QUERY);
  m_connection->prepare(FIND_ONE_QUERY_NAME, FIND_ONE_QUERY);
  m_connection->prepare(FIND_SLOTS_QUERY_NAME, FIND_SLOTS_QUERY);
}

auto PlayerShipRepository::findOneById(const Uuid &ship) const -> PlayerShip
{
  auto out = fetchShipBase(ship);
  fetchSlots(ship, out);

  return out;
}

auto PlayerShipRepository::findAllByPlayer(const Uuid &player) const -> std::unordered_set<Uuid>
{
  auto work       = m_connection->nonTransaction();
  const auto rows = work.exec_prepared(FIND_ALL_QUERY_NAME, toDbId(player));

  std::unordered_set<Uuid> out;
  for (const auto record : rows)
  {
    out.emplace(fromDbId(record[0].as<int>()));
  }

  return out;
}

auto PlayerShipRepository::fetchShipBase(const Uuid &ship) const -> PlayerShip
{
  auto work         = m_connection->nonTransaction();
  const auto record = work.exec_prepared1(FIND_ONE_QUERY_NAME, toDbId(ship));

  PlayerShip out;

  out.faction   = fromDbFaction(record[0].as<std::string>());
  out.shipClass = fromDbShipClass(record[1].as<std::string>());
  out.name      = record[2].as<std::string>();
  if (!record[3].is_null())
  {
    out.player = {fromDbId(record[3].as<int>())};
  }
  out.active          = record[4].as<bool>();
  out.hullPoints      = record[5].as<float>();
  out.maxHullPoints   = record[6].as<float>();
  out.hullPointsRegen = record[7].as<float>();
  out.powerPoints     = record[8].as<float>();
  out.maxPowerPoints  = record[9].as<float>();
  out.powerRegen      = record[10].as<float>();
  out.acceleration    = record[11].as<float>();
  out.speed           = record[12].as<float>();
  out.radius          = record[13].as<float>();

  const auto x = record[14].as<float>();
  const auto y = record[15].as<float>();
  const auto z = record[16].as<float>();
  out.position = Eigen::Vector3f(x, y, z);

  out.jumpTime         = utils::Milliseconds(record[17].as<int>());
  out.jumpTimeInThreat = utils::Milliseconds(record[18].as<int>());

  return out;
}

void PlayerShipRepository::fetchSlots(const Uuid &ship, PlayerShip &out) const
{
  auto work       = m_connection->nonTransaction();
  const auto rows = work.exec_prepared(FIND_SLOTS_QUERY_NAME, toDbId(ship));

  for (const auto record : rows)
  {
    const auto slot  = fromDbSlot(record[0].as<std::string>());
    const auto count = record[1].as<int>();

    out.slots[slot] = count;
  }
}

} // namespace bsgo
