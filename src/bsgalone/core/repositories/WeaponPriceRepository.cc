
#include "WeaponPriceRepository.hh"

namespace bsgo {

WeaponPriceRepository::WeaponPriceRepository(const DbConnectionShPtr &connection)
  : AbstractRepository("weapon", connection)
{
  addModule("price");
}

namespace {
constexpr auto FIND_ALL_QUERY_NAME = "weapon_price_find_all";
constexpr auto FIND_ALL_QUERY      = "SELECT resource, cost FROM weapon_price WHERE weapon = $1";
} // namespace

void WeaponPriceRepository::initialize()
{
  m_connection->prepare(FIND_ALL_QUERY_NAME, FIND_ALL_QUERY);
}

auto WeaponPriceRepository::findAllByWeapon(const Uuid weapon) const
  -> std::unordered_map<Uuid, int>
{
  const auto query = [weapon](pqxx::nontransaction &work) {
    return work.exec(pqxx::prepped{FIND_ALL_QUERY_NAME}, pqxx::params{toDbId(weapon)});
  };
  const auto rows = m_connection->executeQuery(query);

  std::unordered_map<Uuid, int> out;
  for (const auto record : rows)
  {
    const auto resource = fromDbId(record[0].as<int>());
    const auto cost     = record[1].as<int>();

    out[resource] = cost;
  }

  return out;
}

} // namespace bsgo
