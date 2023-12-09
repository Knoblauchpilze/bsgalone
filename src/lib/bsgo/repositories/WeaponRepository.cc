

#include "WeaponRepository.hh"

namespace bsgo {

WeaponRepository::WeaponRepository(const DbConnectionShPtr &connection)
  : AbstractRepository("weapon", connection)
{}

namespace {
constexpr auto FIND_ALL_QUERY_NAME = "weapon_find_all";
constexpr auto FIND_ALL_QUERY      = "SELECT id FROM weapon";

constexpr auto FIND_ONE_QUERY_NAME = "weapon_find_one";
constexpr auto FIND_ONE_QUERY
  = "SELECT name, min_damage, max_damage, power_cost, range, reload_time_ms FROM weapon WHERE id = $1";
} // namespace

void WeaponRepository::initialize()
{
  m_connection->prepare(FIND_ALL_QUERY_NAME, FIND_ALL_QUERY);
  m_connection->prepare(FIND_ONE_QUERY_NAME, FIND_ONE_QUERY);
}

auto WeaponRepository::findAll() const -> std::unordered_set<Uuid>
{
  auto work       = m_connection->nonTransaction();
  const auto rows = work.exec_prepared(FIND_ALL_QUERY_NAME);

  std::unordered_set<Uuid> out;
  for (const auto record : rows)
  {
    out.emplace(fromDbId(record[0].as<int>()));
  }

  return out;
}

auto WeaponRepository::findOneById(const Uuid &weapon) const -> Weapon
{
  auto work         = m_connection->nonTransaction();
  const auto record = work.exec_prepared1(FIND_ONE_QUERY_NAME, toDbId(weapon));

  Weapon out{};

  out.id         = weapon;
  out.name       = record[0].as<std::string>();
  out.minDamage  = record[1].as<float>();
  out.maxDamage  = record[2].as<float>();
  out.powerCost  = record[3].as<float>();
  out.range      = record[4].as<float>();
  out.reloadTime = utils::Milliseconds(record[5].as<int>());

  return out;
}

namespace {
constexpr auto SQL_PURCHASE_PROCEDURE_NAME = "player_buy_weapon";
auto generatePurchaseSqlQuery(const Uuid &player, const Uuid &weapon) -> std::string
{
  std::string out = "CALL ";
  out += SQL_PURCHASE_PROCEDURE_NAME;

  out += " (";
  out += "\'" + std::to_string(toDbId(player)) + "\'";
  out += ",";
  out += "\'" + std::to_string(toDbId(weapon)) + "\'";
  out += ")";

  return out;
}
} // namespace
bool WeaponRepository::saveForPlayer(const Uuid &player, const Uuid &weapon)
{
  const auto sql = generatePurchaseSqlQuery(player, weapon);

  const auto result = m_connection->tryExecuteQuery(sql);
  if (result.error)
  {
    warn("Failed to purchase weapon: " + *result.error);
    return false;
  }

  return true;
}

} // namespace bsgo
