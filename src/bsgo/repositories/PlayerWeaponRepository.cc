
#include "PlayerWeaponRepository.hh"

namespace bsgo {

PlayerWeaponRepository::PlayerWeaponRepository(const DbConnectionShPtr &connection)
  : AbstractRepository("weapon", connection)
{
  addModule("player");
}

namespace {
constexpr auto FIND_ALL_QUERY_NAME = "player_weapon_find_all";
constexpr auto FIND_ALL_QUERY      = "SELECT id FROM player_weapon WHERE player = $1";

constexpr auto FIND_ONE_QUERY_NAME = "player_weapon_find_one";
constexpr auto FIND_ONE_QUERY
  = "SELECT pw.weapon, pw.player, w.name, w.min_damage, w.max_damage, w.power_cost, w.range, w.reload_time_ms, pw.level FROM player_weapon AS pw LEFT JOIN weapon AS w ON pw.weapon = w.id WHERE pw.id = $1";

constexpr auto UPDATE_WEAPON_QUERY_NAME = "player_weapon_update";
constexpr auto UPDATE_WEAPON_QUERY      = R"(
INSERT INTO player_weapon (weapon, player, level)
  VALUES ($1, $2, 1)
)";
} // namespace

void PlayerWeaponRepository::initialize()
{
  m_connection->prepare(FIND_ALL_QUERY_NAME, FIND_ALL_QUERY);
  m_connection->prepare(FIND_ONE_QUERY_NAME, FIND_ONE_QUERY);
  m_connection->prepare(UPDATE_WEAPON_QUERY_NAME, UPDATE_WEAPON_QUERY);
}

auto PlayerWeaponRepository::findOneById(const Uuid weapon) const -> PlayerWeapon
{
  auto work         = m_connection->nonTransaction();
  const auto record = work.exec_prepared1(FIND_ONE_QUERY_NAME, toDbId(weapon));

  PlayerWeapon out{};

  out.id     = weapon;
  out.weapon = fromDbId(record[0].as<int>());
  if (!record[1].is_null())
  {
    out.player = fromDbId(record[1].as<int>());
  }
  out.name       = record[2].as<std::string>();
  out.minDamage  = record[3].as<float>();
  out.maxDamage  = record[4].as<float>();
  out.powerCost  = record[5].as<float>();
  out.range      = record[6].as<float>();
  out.reloadTime = utils::Milliseconds(record[7].as<int>());
  out.level      = record[8].as<int>();

  return out;
}

auto PlayerWeaponRepository::findAllByPlayer(const Uuid player) const -> std::unordered_set<Uuid>
{
  auto work       = m_connection->nonTransaction();
  const auto rows = work.exec_prepared(FIND_ALL_QUERY_NAME, toDbId(player));

  std::unordered_set<Uuid> out;
  for (const auto record : rows)
  {
    out.emplace(fromDbId(record[0].as<int>()));
  }

  return out;
}

void PlayerWeaponRepository::save(const PlayerWeapon &weapon)
{
  auto query = [&weapon](pqxx::work &transaction) {
    if (weapon.player)
    {
      return transaction.exec_prepared0(UPDATE_WEAPON_QUERY_NAME,
                                        toDbId(weapon.weapon),
                                        toDbId(*weapon.player));
    }

    return transaction.exec_prepared0(UPDATE_WEAPON_QUERY_NAME, toDbId(weapon.weapon), nullptr);
  };

  const auto res = m_connection->tryExecuteTransaction(query);
  if (res.error)
  {
    error("Failed to save player weapon: " + *res.error);
  }
}

} // namespace bsgo
