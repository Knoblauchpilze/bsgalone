
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
} // namespace

void ShipWeaponRepository::initialize()
{
  m_connection->prepare(FIND_ALL_QUERY_NAME, FIND_ALL_QUERY);
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
    data.slot   = record[1].as<int>();

    const auto x      = record[2].as<float>();
    const auto y      = record[3].as<float>();
    const auto z      = record[4].as<float>();
    data.slotPosition = Eigen::Vector3f(x, y, z);

    out.emplace_back(std::move(data));
  }

  return out;
}

void ShipWeaponRepository::save(const ShipWeapon & /*weapon*/) {}

} // namespace bsgo
