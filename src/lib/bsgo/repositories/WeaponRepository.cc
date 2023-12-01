

#include "WeaponRepository.hh"

namespace bsgo {

WeaponRepository::WeaponRepository(const DbConnectionShPtr &connection)
  : AbstractRepository("weapon", connection)
{}

namespace {
constexpr auto SQL_QUERY_ALL = "SELECT id FROM weapon";

constexpr auto SQL_QUERY
  = "SELECT name, min_damage, max_damage, power_cost, range, reload_time_ms FROM weapon WHERE id = ";
auto generateSqlQuery(const Uuid &weapon) -> std::string
{
  return SQL_QUERY + std::to_string(toDbId(weapon));
}
} // namespace

auto WeaponRepository::findAll() const -> std::unordered_set<Uuid>
{
  pqxx::nontransaction work(m_connection->connection());
  pqxx::result rows(work.exec(SQL_QUERY_ALL));

  std::unordered_set<Uuid> out;
  for (const auto record : rows)
  {
    out.emplace(fromDbId(record[0].as<int>()));
  }

  return out;
}

auto WeaponRepository::findOneById(const Uuid &weapon) const -> Weapon
{
  const auto sql = generateSqlQuery(weapon);

  pqxx::nontransaction work(m_connection->connection());
  pqxx::result rows(work.exec(sql));

  if (rows.size() != 1)
  {
    error("Expected to find only one weapon with id " + str(weapon));
  }

  Weapon out{};

  const auto &record = rows[0];
  out.id             = weapon;
  out.name           = record[0].as<std::string>();
  out.minDamage      = record[1].as<float>();
  out.maxDamage      = record[2].as<float>();
  out.powerCost      = record[3].as<float>();
  out.range          = record[4].as<float>();
  out.reloadTime     = utils::Milliseconds(record[5].as<int>());

  return out;
}

} // namespace bsgo
