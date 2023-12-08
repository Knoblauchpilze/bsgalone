
#include "WeaponPriceRepository.hh"

#include <iostream>

namespace bsgo {

WeaponPriceRepository::WeaponPriceRepository(const DbConnectionShPtr &connection)
  : AbstractRepository("weapon", connection)
{
  addModule("price");
}

namespace {
constexpr auto SQL_QUERY = "SELECT resource, cost FROM weapon_price WHERE weapon = ";
auto generateSqlQuery(const Uuid &weapon) -> std::string
{
  return SQL_QUERY + std::to_string(toDbId(weapon));
}
} // namespace

auto WeaponPriceRepository::findAllByWeapon(const Uuid &weapon) const
  -> std::unordered_map<Uuid, int>
{
  const auto sql = generateSqlQuery(weapon);

  pqxx::nontransaction work(m_connection->connection());
  const auto rows(work.exec(sql));

  std::unordered_map<Uuid, int> out;
  for (const auto record : rows)
  {
    const auto resource = fromDbId(record[0].as<int>());
    const auto cost     = record[1].as<float>();

    out[resource] = cost;
  }

  return out;
}

} // namespace bsgo
