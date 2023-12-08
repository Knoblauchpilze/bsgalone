
#include "ShipWeaponRepository.hh"

namespace bsgo {

ShipWeaponRepository::ShipWeaponRepository(const DbConnectionShPtr &connection)
  : AbstractRepository("weapon", connection)
{
  addModule("ship");
}

namespace {
constexpr auto SQL_QUERY
  = "SELECT sw.weapon, ss.x_pos, ss.y_pos, ss.z_pos FROM ship_weapon AS sw LEFT JOIN ship_slot AS ss ON sw.slot = ss.id WHERE sw.ship = ";
auto generateSqlQuery(const Uuid &ship) -> std::string
{
  return SQL_QUERY + std::to_string(toDbId(ship));
}
} // namespace

auto ShipWeaponRepository::findAllByShip(const Uuid &ship) const -> std::vector<ShipWeapon>
{
  const auto sql = generateSqlQuery(ship);

  pqxx::nontransaction work(m_connection->connection());
  const auto rows(work.exec(sql));

  std::vector<ShipWeapon> out;
  for (const auto record : rows)
  {
    const auto id = fromDbId(record[0].as<int>());

    const auto x       = record[1].as<float>();
    const auto y       = record[2].as<float>();
    const auto z       = record[3].as<float>();
    const auto slotPos = Eigen::Vector3f(x, y, z);

    out.emplace_back(id, slotPos);
  }

  return out;
}

} // namespace bsgo
