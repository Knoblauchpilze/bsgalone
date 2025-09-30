
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
constexpr auto FIND_ONE_QUERY      = R"(
SELECT
  s.faction,
  s.class,
  s.id,
  ps.name,
  ps.player,
  ps.active,
  ps.hull_points,
  s.max_hull_points,
  s.hull_points_regen,
  ps.power_points,
  s.max_power_points,
  s.power_points_regen,
  s.max_acceleration,
  s.max_speed,
  s.radius,
  ps.x_pos,
  ps.y_pos,
  ps.z_pos,
  ss.system,
  ss.docked,
  sc.jump_time,
  sc.jump_time_threat,
  sj.system
FROM
  player_ship AS ps
  LEFT JOIN ship AS s ON ps.ship = s.id
  LEFT JOIN ship_class AS sc ON s.class = sc.name
  LEFT JOIN ship_system AS ss ON ps.id = ss.ship
  LEFT JOIN ship_jump AS sj ON ps.id = sj.ship
WHERE
  ps.id = $1;
)";

constexpr auto FIND_ONE_BY_PLAYER_AND_ACTIVE_QUERY_NAME = "player_ship_find_one_by_player_and_active";
constexpr auto FIND_ONE_BY_PLAYER_AND_ACTIVE_QUERY
  = "SELECT id FROM player_ship WHERE player = $1 AND active = 'true'";

constexpr auto FIND_SLOTS_QUERY_NAME = "player_ship_find_slots";
constexpr auto FIND_SLOTS_QUERY      = R"(
SELECT
  ss.type,
  COUNT(ss.id)
FROM
  player_ship AS ps
  LEFT JOIN ship AS s ON ps.ship = s.id
  LEFT JOIN ship_slot AS ss ON s.id = ss.ship
WHERE
  ps.id = $1
GROUP BY
  ss.type
)";

constexpr auto FIND_EMPTY_WEAPON_SLOTS_QUERY_NAME = "player_ship_find_empty_slots";
constexpr auto FIND_EMPTY_WEAPON_SLOTS_QUERY      = R"(
SELECT
  ss.id
FROM
  ship AS s
  LEFT JOIN ship_slot AS ss ON s.id = ss.ship
  LEFT JOIN player_ship AS ps ON ps.ship = s.id
  LEFT JOIN ship_weapon AS sw ON sw.ship = ps.id AND sw.slot = ss.id
WHERE
  ps.id = $1
  AND ss.type = 'weapon'
  AND sw.weapon IS NULL
)";

constexpr auto CREATE_SHIP_QUERY_NAME = "player_ship_create";
constexpr auto CREATE_SHIP_QUERY      = R"(
INSERT INTO player_ship (ship, player, name, active, hull_points, power_points, x_pos, y_pos, z_pos)
  VALUES ($1, $2, $3, $4, $5, $6, 0, 0, 0)
)";

constexpr auto UPDATE_SHIP_QUERY_NAME = "player_ship_update";
constexpr auto UPDATE_SHIP_QUERY      = R"(
UPDATE player_ship
  SET
    name = $1,
    active = $2,
    hull_points = $3,
    power_points = $4,
    x_pos = $5,
    y_pos = $6,
    z_pos = $7
  WHERE
    id = $8
)";

constexpr auto UPDATE_SHIP_JUMP_QUERY_NAME = "player_ship_update_jump";
constexpr auto UPDATE_SHIP_JUMP_QUERY      = R"(
INSERT INTO ship_jump (ship, system)
  VALUES ($1, $2)
  ON CONFLICT (ship, system) DO UPDATE
  SET
    system = excluded.system
  WHERE
    ship_jump.ship = excluded.ship
)";

constexpr auto CANCEL_SHIP_JUMP_QUERY_NAME = "player_ship_cancel_jump";
constexpr auto CANCEL_SHIP_JUMP_QUERY      = "DELETE FROM ship_jump WHERE ship = $1";
} // namespace

void PlayerShipRepository::initialize()
{
  m_connection->prepare(FIND_ALL_QUERY_NAME, FIND_ALL_QUERY);
  m_connection->prepare(FIND_ONE_QUERY_NAME, FIND_ONE_QUERY);
  m_connection->prepare(FIND_ONE_BY_PLAYER_AND_ACTIVE_QUERY_NAME,
                        FIND_ONE_BY_PLAYER_AND_ACTIVE_QUERY);
  m_connection->prepare(FIND_SLOTS_QUERY_NAME, FIND_SLOTS_QUERY);
  m_connection->prepare(FIND_EMPTY_WEAPON_SLOTS_QUERY_NAME, FIND_EMPTY_WEAPON_SLOTS_QUERY);
  m_connection->prepare(CREATE_SHIP_QUERY_NAME, CREATE_SHIP_QUERY);
  m_connection->prepare(UPDATE_SHIP_QUERY_NAME, UPDATE_SHIP_QUERY);
  m_connection->prepare(UPDATE_SHIP_JUMP_QUERY_NAME, UPDATE_SHIP_JUMP_QUERY);
  m_connection->prepare(CANCEL_SHIP_JUMP_QUERY_NAME, CANCEL_SHIP_JUMP_QUERY);
}

auto PlayerShipRepository::findOneById(const Uuid ship) const -> PlayerShip
{
  auto out = fetchShipBase(ship);
  fetchSlots(ship, out);

  return out;
}

auto PlayerShipRepository::findOneByPlayerAndActive(const Uuid player) const -> PlayerShip
{
  const auto query = [player](pqxx::nontransaction &work) {
    return work
      .exec(pqxx::prepped{FIND_ONE_BY_PLAYER_AND_ACTIVE_QUERY_NAME}, pqxx::params{toDbId(player)})
      .one_row();
  };
  const auto record = m_connection->executeQueryReturningSingleRow(query);

  const auto shipId = fromDbId(record[0].as<int>());

  auto out = fetchShipBase(shipId);
  fetchSlots(shipId, out);

  return out;
}

auto PlayerShipRepository::findAllByPlayer(const Uuid player) const -> std::unordered_set<Uuid>
{
  const auto query = [player](pqxx::nontransaction &work) {
    return work.exec(pqxx::prepped{FIND_ALL_QUERY_NAME}, pqxx::params{toDbId(player)});
  };
  const auto rows = m_connection->executeQuery(query);

  std::unordered_set<Uuid> out;
  for (const auto record : rows)
  {
    out.emplace(fromDbId(record[0].as<int>()));
  }

  return out;
}

auto PlayerShipRepository::findAllAvailableWeaponSlotByShip(const Uuid ship) -> std::set<Uuid>
{
  const auto query = [ship](pqxx::nontransaction &work) {
    return work.exec(pqxx::prepped{FIND_EMPTY_WEAPON_SLOTS_QUERY_NAME}, pqxx::params{toDbId(ship)});
  };
  const auto rows = m_connection->executeQuery(query);

  std::set<Uuid> out;
  for (const auto record : rows)
  {
    out.emplace(fromDbId(record[0].as<int>()));
  }

  return out;
}

void PlayerShipRepository::create(const PlayerShip &ship)
{
  auto query = [&ship](pqxx::work &transaction) {
    return transaction
      .exec(pqxx::prepped{CREATE_SHIP_QUERY_NAME},
            pqxx::params{toDbId(ship.ship),
                         toDbId(*ship.player),
                         ship.name,
                         ship.active,
                         ship.hullPoints,
                         ship.powerPoints})
      .no_rows();
  };

  auto res = m_connection->tryExecuteTransaction(query);
  if (res.error)
  {
    error("Failed to create player ship: " + *res.error);
  }
}

void PlayerShipRepository::save(const PlayerShip &ship)
{
  auto query = [&ship](pqxx::work &transaction) {
    return transaction
      .exec(pqxx::prepped{UPDATE_SHIP_QUERY_NAME},
            pqxx::params{ship.name,
                         ship.active,
                         ship.hullPoints,
                         ship.powerPoints,
                         ship.position(0),
                         ship.position(1),
                         ship.position(2),
                         toDbId(ship.id)})
      .no_rows();
  };

  auto res = m_connection->tryExecuteTransaction(query);
  if (res.error)
  {
    error("Failed to save player ship: " + *res.error);
  }
}

void PlayerShipRepository::saveJump(const Uuid shipDbId, const std::optional<Uuid> jumpSystem)
{
  if (jumpSystem)
  {
    registerShipJump(shipDbId, *jumpSystem);
  }
  else
  {
    cancelShipJump(shipDbId);
  }
}

auto PlayerShipRepository::fetchShipBase(const Uuid ship) const -> PlayerShip
{
  const auto query = [ship](pqxx::nontransaction &work) {
    return work.exec(pqxx::prepped{FIND_ONE_QUERY_NAME}, pqxx::params{toDbId(ship)}).one_row();
  };
  const auto record = m_connection->executeQueryReturningSingleRow(query);

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

  if (!record[18].is_null())
  {
    out.system = fromDbId(record[18].as<int>());
  }
  if (!record[19].is_null())
  {
    out.docked = record[19].as<bool>();
  }

  out.jumpTime         = Tick::fromInt(record[20].as<int>());
  out.jumpTimeInThreat = Tick::fromInt(record[21].as<int>());
  if (!record[22].is_null())
  {
    out.jumpSystem = fromDbId(record[22].as<int>());
  }

  return out;
}

void PlayerShipRepository::fetchSlots(const Uuid ship, PlayerShip &out) const
{
  const auto query = [ship](pqxx::nontransaction &work) {
    return work.exec(pqxx::prepped{FIND_SLOTS_QUERY_NAME}, pqxx::params{toDbId(ship)});
  };
  const auto rows = m_connection->executeQuery(query);

  for (const auto record : rows)
  {
    const auto slot  = fromDbSlot(record[0].as<std::string>());
    const auto count = record[1].as<int>();

    out.slots[slot] = count;
  }
}

void PlayerShipRepository::registerShipJump(const Uuid ship, const Uuid system) const
{
  const auto query = [&ship, &system](pqxx::work &transaction) {
    return transaction
      .exec(pqxx::prepped{UPDATE_SHIP_JUMP_QUERY_NAME}, pqxx::params{toDbId(ship), toDbId(system)})
      .no_rows();
  };

  const auto res = m_connection->tryExecuteTransaction(query);
  if (res.error)
  {
    error("Failed to save player ship jump: " + *res.error);
  }
}

void PlayerShipRepository::cancelShipJump(const Uuid ship) const
{
  const auto query = [&ship](pqxx::work &transaction) {
    return transaction.exec(pqxx::prepped{CANCEL_SHIP_JUMP_QUERY_NAME}, pqxx::params{toDbId(ship)})
      .no_rows();
  };

  const auto res = m_connection->tryExecuteTransaction(query);
  if (res.error)
  {
    error("Failed to cancel player ship jump: " + *res.error);
  }
}

} // namespace bsgo
