
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
  const auto query = [weapon](pqxx::nontransaction &work) {
    return work.exec(pqxx::prepped{FIND_ONE_QUERY_NAME}, pqxx::params{toDbId(weapon)}).one_row();
  };
  const auto record = m_connection->executeQueryReturningSingleRow(query);

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
  out.reloadTime = core::Milliseconds(record[7].as<int>());
  out.level      = record[8].as<int>();

  return out;
}

auto PlayerWeaponRepository::findAllByPlayer(const Uuid player) const -> std::unordered_set<Uuid>
{
  const auto query = [player](pqxx::nontransaction &work) {
    return work.exec(pqxx::prepped{FIND_ALL_QUERY_NAME}, pqxx::params{toDbId(player)});
  };
  const auto rows = m_connection->executeQuery(query);

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
      return transaction
        .exec(pqxx::prepped{UPDATE_WEAPON_QUERY_NAME},
              pqxx::params{toDbId(weapon.weapon), toDbId(*weapon.player)})
        .no_rows();
    }

    return transaction
      .exec(pqxx::prepped{UPDATE_WEAPON_QUERY_NAME}, pqxx::params{toDbId(weapon.weapon), nullptr})
      .no_rows();
  };

  const auto res = m_connection->tryExecuteTransaction(query);
  if (res.error)
  {
    error("Failed to save player weapon: " + *res.error);
  }
}

} // namespace bsgo
