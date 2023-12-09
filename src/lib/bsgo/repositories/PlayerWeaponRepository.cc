
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
  = "SELECT w.name, w.min_damage, w.max_damage, w.power_cost, w.range, w.reload_time_ms, pw.level FROM player_weapon AS pw LEFT JOIN weapon AS w ON pw.weapon = w.id WHERE pw.id = $1";
} // namespace

void PlayerWeaponRepository::initialize()
{
  m_connection->prepare(FIND_ALL_QUERY_NAME, FIND_ALL_QUERY);
  m_connection->prepare(FIND_ONE_QUERY_NAME, FIND_ONE_QUERY);
}

auto PlayerWeaponRepository::findOneById(const Uuid &weapon) const -> PlayerWeapon
{
  auto work         = m_connection->nonTransaction();
  const auto record = work.exec_prepared1(FIND_ONE_QUERY_NAME, toDbId(weapon));

  PlayerWeapon out{};

  out.id         = weapon;
  out.name       = record[0].as<std::string>();
  out.minDamage  = record[1].as<float>();
  out.maxDamage  = record[2].as<float>();
  out.powerCost  = record[3].as<float>();
  out.range      = record[4].as<float>();
  out.reloadTime = utils::Milliseconds(record[5].as<int>());
  out.level      = record[6].as<int>();

  return out;
}

auto PlayerWeaponRepository::findAllByPlayer(const Uuid &player) const -> std::unordered_set<Uuid>
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

void PlayerWeaponRepository::save(const PlayerWeapon & /*weapon*/) {}

} // namespace bsgo
