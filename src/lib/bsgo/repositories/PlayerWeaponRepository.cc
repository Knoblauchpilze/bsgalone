
#include "PlayerWeaponRepository.hh"

namespace bsgo {

PlayerWeaponRepository::PlayerWeaponRepository(const DbConnectionShPtr &connection)
  : AbstractRepository("weapon", connection)
{
  addModule("player");
}

namespace {
constexpr auto SQL_QUERY_ALL = "SELECT id FROM player_weapon WHERE player = ";
auto generateAllSqlQuery(const Uuid &player) -> std::string
{
  return SQL_QUERY_ALL + std::to_string(toDbId(player));
}

constexpr auto SQL_QUERY
  = "SELECT w.name, w.min_damage, w.max_damage, w.power_cost, w.range, w.reload_time_ms, pw.level FROM player_weapon AS pw LEFT JOIN weapon AS w ON pw.weapon = w.id WHERE pw.id = ";
auto generateSqlQuery(const Uuid &weapon) -> std::string
{
  return SQL_QUERY + std::to_string(toDbId(weapon));
}
} // namespace

auto PlayerWeaponRepository::findOneById(const Uuid &weapon) const -> PlayerWeapon
{
  const auto sql = generateSqlQuery(weapon);

  pqxx::nontransaction work(m_connection->connection());
  const auto rows(work.exec(sql));

  if (rows.size() != 1)
  {
    error("Expected to find only one weapon with id " + str(weapon));
  }

  PlayerWeapon out{};

  const auto &record = rows[0];
  out.id             = weapon;
  out.name           = record[0].as<std::string>();
  out.minDamage      = record[1].as<float>();
  out.maxDamage      = record[2].as<float>();
  out.powerCost      = record[3].as<float>();
  out.range          = record[4].as<float>();
  out.reloadTime     = utils::Milliseconds(record[5].as<int>());
  out.level          = record[6].as<int>();

  return out;
}

auto PlayerWeaponRepository::findAllByPlayer(const Uuid &player) const -> std::unordered_set<Uuid>
{
  const auto sql = generateAllSqlQuery(player);

  pqxx::nontransaction work(m_connection->connection());
  const auto rows(work.exec(sql));

  std::unordered_set<Uuid> out;
  for (const auto record : rows)
  {
    out.emplace(fromDbId(record[0].as<int>()));
  }

  return out;
}

} // namespace bsgo
