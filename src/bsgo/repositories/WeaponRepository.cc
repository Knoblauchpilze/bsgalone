

#include "WeaponRepository.hh"

namespace bsgo {

WeaponRepository::WeaponRepository(const DbConnectionShPtr &connection)
  : AbstractRepository("weapon", connection)
{}

namespace {
constexpr auto FIND_ALL_QUERY_NAME = "weapon_find_all";
constexpr auto FIND_ALL_QUERY      = "SELECT id FROM weapon";

constexpr auto FIND_ONE_QUERY_NAME = "weapon_find_one";
constexpr auto FIND_ONE_QUERY      = R"(
SELECT
  name,
  min_damage,
  max_damage,
  power_cost,
  range,
  reload_time
FROM
  weapon
WHERE
  id = $1
)";
} // namespace

void WeaponRepository::initialize()
{
  m_connection->prepare(FIND_ALL_QUERY_NAME, FIND_ALL_QUERY);
  m_connection->prepare(FIND_ONE_QUERY_NAME, FIND_ONE_QUERY);
}

auto WeaponRepository::findAll() const -> std::unordered_set<Uuid>
{
  const auto query = [](pqxx::nontransaction &work) {
    return work.exec(pqxx::prepped{FIND_ALL_QUERY_NAME});
  };
  const auto rows = m_connection->executeQuery(query);

  std::unordered_set<Uuid> out;
  for (const auto record : rows)
  {
    out.emplace(fromDbId(record[0].as<int>()));
  }

  return out;
}

auto WeaponRepository::findOneById(const Uuid weapon) const -> Weapon
{
  const auto query = [weapon](pqxx::nontransaction &work) {
    return work.exec(pqxx::prepped{FIND_ONE_QUERY_NAME}, pqxx::params{toDbId(weapon)}).one_row();
  };
  const auto record = m_connection->executeQueryReturningSingleRow(query);

  Weapon out{};

  out.id         = weapon;
  out.name       = record[0].as<std::string>();
  out.minDamage  = record[1].as<float>();
  out.maxDamage  = record[2].as<float>();
  out.powerCost  = record[3].as<float>();
  out.range      = record[4].as<float>();
  out.reloadTime = TickDuration::fromInt(record[5].as<int>());

  return out;
}

} // namespace bsgo
