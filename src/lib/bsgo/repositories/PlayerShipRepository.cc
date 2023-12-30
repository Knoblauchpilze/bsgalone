
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
  = "SELECT s.faction, s.class, s.id, ps.name, ps.player, ps.active, ps.hull_points, s.max_hull_points, s.hull_points_regen, ps.power_points, s.max_power_points, s.power_points_regen, s.max_acceleration, s.max_speed, s.radius, ps.x_pos, ps.y_pos, ps.z_pos, sc.jump_time_ms, sc.jump_time_threat_ms, sj.system FROM player_ship AS ps LEFT JOIN ship AS s ON ps.ship = s.id LEFT JOIN ship_class AS sc ON s.class = sc.name LEFT JOIN ship_jump AS sj ON ps.id = sj.ship WHERE ps.id = $1";

constexpr auto FIND_ONE_BY_PLAYER_AND_ACTIVE_QUERY_NAME = "player_ship_find_one_by_player_and_active";
constexpr auto FIND_ONE_BY_PLAYER_AND_ACTIVE_QUERY
  = "SELECT id FROM player_ship WHERE player = $1 AND active = 'true'";

constexpr auto FIND_SLOTS_QUERY_NAME = "player_ship_find_slots";
constexpr auto FIND_SLOTS_QUERY
  = "SELECT ss.type, COUNT(ss.id) FROM player_ship AS ps LEFT JOIN ship AS s ON ps.ship = s.id LEFT JOIN ship_slot AS ss ON s.id = ss.ship WHERE ps.id = $1 GROUP BY ss.type";

constexpr auto FIND_EMPTY_WEAPON_SLOTS_QUERY_NAME = "player_ship_find_empty_slots";
constexpr auto FIND_EMPTY_WEAPON_SLOTS_QUERY
  = "select ss.id FROM ship AS s LEFT JOIN ship_slot AS ss ON s.id = ss.ship LEFT JOIN player_ship AS ps ON ps.ship = s.id LEFT JOIN ship_weapon AS sw ON sw.ship = ps.id AND sw.slot = ss.id WHERE ps.id = $1 AND ss.type = 'weapon' AND sw.weapon IS NULL";

constexpr auto UPDATE_SHIP_QUERY_NAME = "player_ship_update";
constexpr auto UPDATE_SHIP_QUERY      = R"(
INSERT INTO player_ship (ship, player, name, active, hull_points, power_points, x_pos, y_pos, z_pos)
  VALUES ($1, $2, $3, $4, $5, $6, 0, 0, 0)
  ON CONFLICT (ship, player) DO UPDATE
  SET
    name = excluded.name,
    active = excluded.active,
    hull_points = excluded.hull_points,
    power_points = excluded.power_points,
    x_pos = excluded.x_pos,
    y_pos = excluded.y_pos,
    z_pos = excluded.z_pos
  WHERE
    player_ship.ship = excluded.ship
    AND player_ship.player = excluded.player
)";
} // namespace

void PlayerShipRepository::initialize()
{
  m_connection->prepare(FIND_ALL_QUERY_NAME, FIND_ALL_QUERY);
  m_connection->prepare(FIND_ONE_QUERY_NAME, FIND_ONE_QUERY);
  m_connection->prepare(FIND_ONE_BY_PLAYER_AND_ACTIVE_QUERY_NAME,
                        FIND_ONE_BY_PLAYER_AND_ACTIVE_QUERY);
  m_connection->prepare(FIND_SLOTS_QUERY_NAME, FIND_SLOTS_QUERY);
  m_connection->prepare(FIND_EMPTY_WEAPON_SLOTS_QUERY_NAME, FIND_EMPTY_WEAPON_SLOTS_QUERY);
  m_connection->prepare(UPDATE_SHIP_QUERY_NAME, UPDATE_SHIP_QUERY);
}

auto PlayerShipRepository::findOneById(const Uuid &ship) const -> PlayerShip
{
  auto out = fetchShipBase(ship);
  fetchSlots(ship, out);

  return out;
}

auto PlayerShipRepository::findOneByPlayerAndActive(const Uuid &player) const -> PlayerShip
{
  Uuid shipId{};

  {
    auto work         = m_connection->nonTransaction();
    const auto record = work.exec_prepared1(FIND_ONE_BY_PLAYER_AND_ACTIVE_QUERY_NAME,
                                            toDbId(player));
    shipId            = fromDbId(record[0].as<int>());
  }

  auto out = fetchShipBase(shipId);
  fetchSlots(shipId, out);

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

auto PlayerShipRepository::findAllAvailableWeaponSlotByShip(const Uuid &ship) -> std::set<Uuid>
{
  auto work       = m_connection->nonTransaction();
  const auto rows = work.exec_prepared(FIND_EMPTY_WEAPON_SLOTS_QUERY_NAME, toDbId(ship));

  std::set<Uuid> out;
  for (const auto record : rows)
  {
    out.emplace(fromDbId(record[0].as<int>()));
  }

  return out;
}

void PlayerShipRepository::save(const PlayerShip &ship)
{
  auto query = [&ship](pqxx::work &transaction) {
    return transaction.exec_prepared0(UPDATE_SHIP_QUERY_NAME,
                                      toDbId(ship.ship),
                                      toDbId(*ship.player),
                                      ship.name,
                                      ship.active,
                                      ship.hullPoints,
                                      ship.powerPoints);
  };

  const auto res = m_connection->tryExecuteTransaction(query);
  if (res.error)
  {
    error("Failed to save player ship: " + *res.error);
  }
}

auto PlayerShipRepository::fetchShipBase(const Uuid &ship) const -> PlayerShip
{
  auto work         = m_connection->nonTransaction();
  const auto record = work.exec_prepared1(FIND_ONE_QUERY_NAME, toDbId(ship));

  PlayerShip out;

  out.id        = ship;
  out.faction   = fromDbFaction(record[0].as<std::string>());
  out.shipClass = fromDbShipClass(record[1].as<std::string>());
  out.ship      = fromDbId(record[2].as<int>());
  out.name      = record[3].as<std::string>();
  if (!record[4].is_null())
  {
    out.player = fromDbId(record[4].as<int>());
  }
  out.active          = record[5].as<bool>();
  out.hullPoints      = record[6].as<float>();
  out.maxHullPoints   = record[7].as<float>();
  out.hullPointsRegen = record[8].as<float>();
  out.powerPoints     = record[9].as<float>();
  out.maxPowerPoints  = record[10].as<float>();
  out.powerRegen      = record[11].as<float>();
  out.acceleration    = record[12].as<float>();
  out.speed           = record[13].as<float>();
  out.radius          = record[14].as<float>();

  const auto x = record[15].as<float>();
  const auto y = record[16].as<float>();
  const auto z = record[17].as<float>();
  out.position = Eigen::Vector3f(x, y, z);

  out.jumpTime         = utils::Milliseconds(record[18].as<int>());
  out.jumpTimeInThreat = utils::Milliseconds(record[19].as<int>());
  if (!record[20].is_null())
  {
    out.jumpSystem = fromDbId(record[20].as<int>());
  }

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
