
#include "ShipWeaponRepository.hh"

namespace bsgo {

ShipWeaponRepository::ShipWeaponRepository(const DbConnectionShPtr &connection)
  : AbstractRepository("weapon", connection)
{
  addModule("ship");
}

namespace {
constexpr auto FIND_ALL_QUERY_NAME = "ship_weapon_find_all";
constexpr auto FIND_ALL_QUERY      = R"(
SELECT
  sw.weapon,
  sw.slot,
  ss.x_pos,
  ss.y_pos,
  ss.z_pos
FROM
  ship_weapon AS sw
  LEFT JOIN ship_slot AS ss ON sw.slot = ss.id
WHERE
  sw.ship = $1
)";

constexpr auto FIND_ONE_QUERY_NAME = "ship_weapon_find_one";
constexpr auto FIND_ONE_QUERY      = R"(
SELECT
  sw.slot,
  ss.x_pos,
  ss.y_pos,
  ss.z_pos
FROM
  ship_weapon AS sw
  LEFT JOIN ship_slot AS ss ON sw.slot = ss.id
WHERE
  sw.ship = $1
  AND sw.weapon = $2
)";

constexpr auto FIND_ONE_BY_WEAPON_QUERY_NAME = "ship_weapon_find_one_by_weapon";
constexpr auto FIND_ONE_BY_WEAPON_QUERY      = R"(
SELECT
  sw.ship,
  sw.slot,
  ss.x_pos,
  ss.y_pos,
  ss.z_pos
FROM
  ship_weapon AS sw
  LEFT JOIN ship_slot AS ss ON sw.slot = ss.id
WHERE
  sw.weapon = $1
)";

constexpr auto UPDATE_WEAPON_QUERY_NAME = "ship_weapon_update";
constexpr auto UPDATE_WEAPON_QUERY      = R"(
INSERT INTO ship_weapon (ship, weapon, slot)
  VALUES ($1, $2, $3)
  ON CONFLICT (ship, weapon) DO UPDATE
  SET
    slot = excluded.slot
  WHERE
    ship_weapon.ship = excluded.ship
    AND ship_weapon.weapon = excluded.weapon
)";

constexpr auto DELETE_WEAPON_QUERY_NAME = "ship_weapon_delete";
constexpr auto DELETE_WEAPON_QUERY      = "DELETE FROM ship_weapon WHERE ship = $1 AND slot = $2";
} // namespace

void ShipWeaponRepository::initialize()
{
  m_connection->prepare(FIND_ALL_QUERY_NAME, FIND_ALL_QUERY);
  m_connection->prepare(FIND_ONE_QUERY_NAME, FIND_ONE_QUERY);
  m_connection->prepare(FIND_ONE_BY_WEAPON_QUERY_NAME, FIND_ONE_BY_WEAPON_QUERY);
  m_connection->prepare(UPDATE_WEAPON_QUERY_NAME, UPDATE_WEAPON_QUERY);
  m_connection->prepare(DELETE_WEAPON_QUERY_NAME, DELETE_WEAPON_QUERY);
}

auto ShipWeaponRepository::findOneByShipAndWeapon(const Uuid ship, const Uuid weapon) const
  -> std::optional<ShipWeapon>
{
  const auto query = [ship, weapon](pqxx::nontransaction &work) {
    return work.exec_prepared(FIND_ONE_QUERY_NAME, toDbId(ship), toDbId(weapon));
  };
  const auto rows = m_connection->executeQuery(query);

  if (rows.empty())
  {
    return {};
  }

  if (rows.size() != 1)
  {
    error("Expected to find only one slot for ship " + str(ship) + " and weapon " + str(weapon));
  }

  ShipWeapon out;

  out.ship   = ship;
  out.weapon = weapon;

  const auto &record = rows[0];
  out.slot           = fromDbId(record[0].as<int>());

  const auto x     = record[1].as<float>();
  const auto y     = record[2].as<float>();
  const auto z     = record[3].as<float>();
  out.slotPosition = Eigen::Vector3f(x, y, z);

  return out;
}

auto ShipWeaponRepository::findOneByWeapon(const Uuid weapon) const -> std::optional<ShipWeapon>
{
  const auto query = [weapon](pqxx::nontransaction &work) {
    return work.exec_prepared(FIND_ONE_BY_WEAPON_QUERY_NAME, toDbId(weapon));
  };
  const auto rows = m_connection->executeQuery(query);

  if (rows.empty())
  {
    return {};
  }

  if (rows.size() != 1)
  {
    error("Expected to find only one slot for weapon " + str(weapon));
  }

  ShipWeapon out;
  out.weapon = weapon;

  const auto &record = rows[0];

  out.ship = fromDbId(record[0].as<int>());
  out.slot = fromDbId(record[1].as<int>());

  const auto x     = record[2].as<float>();
  const auto y     = record[3].as<float>();
  const auto z     = record[4].as<float>();
  out.slotPosition = Eigen::Vector3f(x, y, z);

  return out;
}

auto ShipWeaponRepository::findAllByShip(const Uuid ship) const -> std::vector<ShipWeapon>
{
  const auto query = [ship](pqxx::nontransaction &work) {
    return work.exec_prepared(FIND_ALL_QUERY_NAME, toDbId(ship));
  };
  const auto rows = m_connection->executeQuery(query);

  std::vector<ShipWeapon> out;
  for (const auto record : rows)
  {
    ShipWeapon data{};
    data.ship   = ship;
    data.weapon = fromDbId(record[0].as<int>());
    data.slot   = fromDbId(record[1].as<int>());

    const auto x      = record[2].as<float>();
    const auto y      = record[3].as<float>();
    const auto z      = record[4].as<float>();
    data.slotPosition = Eigen::Vector3f(x, y, z);

    out.emplace_back(std::move(data));
  }

  return out;
}

void ShipWeaponRepository::save(const ShipWeapon &weapon)
{
  auto query = [&weapon](pqxx::work &transaction) {
    return transaction.exec_prepared0(UPDATE_WEAPON_QUERY_NAME,
                                      toDbId(weapon.ship),
                                      toDbId(weapon.weapon),
                                      toDbId(weapon.slot));
  };

  const auto res = m_connection->tryExecuteTransaction(query);
  if (res.error)
  {
    error("Failed to save ship weapon: " + *res.error);
  }
}

void ShipWeaponRepository::deleteByShipAndSlot(const ShipWeapon &weapon)
{
  auto query = [&weapon](pqxx::work &transaction) {
    return transaction.exec_prepared0(DELETE_WEAPON_QUERY_NAME,
                                      toDbId(weapon.ship),
                                      toDbId(weapon.slot));
  };

  const auto res = m_connection->tryExecuteTransaction(query);
  if (res.error)
  {
    error("Failed to delete ship weapon: " + *res.error);
  }
}

} // namespace bsgo
