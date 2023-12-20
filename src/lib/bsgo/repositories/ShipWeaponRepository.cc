
#include "ShipWeaponRepository.hh"

namespace bsgo {

ShipWeaponRepository::ShipWeaponRepository(const DbConnectionShPtr &connection)
  : AbstractRepository("weapon", connection)
{
  addModule("ship");
}

namespace {
constexpr auto FIND_ALL_QUERY_NAME = "ship_weapon_find_all";
constexpr auto FIND_ALL_QUERY
  = "SELECT sw.weapon, sw.slot, ss.x_pos, ss.y_pos, ss.z_pos FROM ship_weapon AS sw LEFT JOIN ship_slot AS ss ON sw.slot = ss.id WHERE sw.ship = $1";

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
  m_connection->prepare(UPDATE_WEAPON_QUERY_NAME, UPDATE_WEAPON_QUERY);
  m_connection->prepare(DELETE_WEAPON_QUERY_NAME, DELETE_WEAPON_QUERY);
}

auto ShipWeaponRepository::findAllByShip(const Uuid &ship) const -> std::vector<ShipWeapon>
{
  auto work       = m_connection->nonTransaction();
  const auto rows = work.exec_prepared(FIND_ALL_QUERY_NAME, toDbId(ship));

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
    return transaction.exec_prepared0(DELETE_WEAPON_QUERY, toDbId(weapon.ship), toDbId(weapon.slot));
  };

  const auto res = m_connection->tryExecuteTransaction(query);
  if (res.error)
  {
    error("Failed to delete ship weapon: " + *res.error);
  }
}

} // namespace bsgo
