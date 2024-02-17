
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
  -> std::unordered_map<Uuid, float>
{
  auto work       = m_connection->nonTransaction();
  const auto rows = work.exec_prepared(FIND_ALL_QUERY_NAME, toDbId(weapon));

  std::unordered_map<Uuid, float> out;
  for (const auto record : rows)
  {
    const auto resource = fromDbId(record[0].as<float>());
    const auto cost     = record[1].as<float>();

    out[resource] = cost;
  }

  return out;
}

} // namespace bsgo
