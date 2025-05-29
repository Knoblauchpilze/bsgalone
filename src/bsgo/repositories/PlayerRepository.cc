
#include "PlayerRepository.hh"

namespace bsgo {

PlayerRepository::PlayerRepository(const DbConnectionShPtr &connection)
  : AbstractRepository("player", connection)
{}

namespace {
constexpr auto FIND_ALL_QUERY_NAME = "player_find_all";
constexpr auto FIND_ALL_QUERY      = "SELECT id FROM system";

constexpr auto FIND_ALL_BY_SYSTEM_QUERY_NAME = "system_find_all_by_system";
constexpr auto FIND_ALL_BY_SYSTEM_QUERY      = R"(
SELECT
  ps.player
FROM
  player_ship AS ps
  LEFT JOIN ship_system AS ss ON ps.id = ss.ship
  LEFT JOIN player AS p ON ps.player = p.id
WHERE
  ps.active = true
  AND ps.player IS NOT NULL
  AND ss.system = $1
)";

constexpr auto FIND_ONE_QUERY_NAME = "player_find_one";
constexpr auto FIND_ONE_QUERY      = "SELECT name, password, faction FROM player WHERE id = $1";

constexpr auto FIND_ONE_BY_NAME_QUERY_NAME = "player_find_one_by_player";
constexpr auto FIND_ONE_BY_NAME_QUERY = "SELECT id, password, faction FROM player WHERE name = $1";

constexpr auto FIND_SYSTEM_QUERY_NAME = "player_find_system";
constexpr auto FIND_SYSTEM_QUERY
  = "SELECT ss.system FROM player_ship AS ps LEFT JOIN ship_system AS ss ON ps.id = ss.ship LEFT JOIN player AS p ON ps.player = p.id WHERE ps.active = true AND ps.player IS NOT NULL AND ps.player = $1";

constexpr auto UPDATE_PLAYER_QUERY_NAME = "player_update";
constexpr auto UPDATE_PLAYER_QUERY      = R"(
INSERT INTO player (name, password, faction)
  VALUES ($1, $2, $3)
)";
} // namespace

void PlayerRepository::initialize()
{
  m_connection->prepare(FIND_ALL_QUERY_NAME, FIND_ALL_QUERY);
  m_connection->prepare(FIND_ALL_BY_SYSTEM_QUERY_NAME, FIND_ALL_BY_SYSTEM_QUERY);
  m_connection->prepare(FIND_ONE_QUERY_NAME, FIND_ONE_QUERY);
  m_connection->prepare(FIND_ONE_BY_NAME_QUERY_NAME, FIND_ONE_BY_NAME_QUERY);
  m_connection->prepare(FIND_SYSTEM_QUERY_NAME, FIND_SYSTEM_QUERY);
  m_connection->prepare(UPDATE_PLAYER_QUERY_NAME, UPDATE_PLAYER_QUERY);
}

auto PlayerRepository::findAll() const -> std::unordered_set<Uuid>
{
  const auto query = [](pqxx::nontransaction &work) { return work.exec(FIND_ALL_QUERY_NAME); };
  const auto rows  = m_connection->executeQuery(query);

  std::unordered_set<Uuid> out;
  for (const auto record : rows)
  {
    out.emplace(fromDbId(record[0].as<int>()));
  }

  return out;
}

auto PlayerRepository::findAllBySystem(const Uuid system) const -> std::unordered_set<Uuid>
{
  const auto query = [system](pqxx::nontransaction &work) {
    // https://libpqxx.readthedocs.io/stable/parameters.html
    return work.exec(pqxx::prepped{FIND_ALL_BY_SYSTEM_QUERY_NAME}, pqxx::params{toDbId(system)});
  };
  const auto rows = m_connection->executeQuery(query);

  std::unordered_set<Uuid> out;
  for (const auto record : rows)
  {
    out.emplace(fromDbId(record[0].as<int>()));
  }

  return out;
}

auto PlayerRepository::findOneById(const Uuid player) const -> Player
{
  const auto query = [player](pqxx::nontransaction &work) {
    return work.exec(pqxx::prepped{FIND_ONE_QUERY_NAME}, pqxx::params{toDbId(player)}).one_row();
  };
  const auto record = m_connection->executeQueryReturningSingleRow(query);

  Player out;

  out.id       = player;
  out.name     = record[0].as<std::string>();
  out.password = record[1].as<std::string>();
  out.faction  = fromDbFaction(record[2].as<std::string>());

  return out;
}

auto PlayerRepository::findOneByName(const std::string &name) const -> std::optional<Player>
{
  const auto query = [name](pqxx::nontransaction &work) {
    return work.exec(pqxx::prepped{FIND_ONE_BY_NAME_QUERY_NAME}, pqxx::params{name});
  };
  const auto rows = m_connection->executeQuery(query);

  if (rows.empty())
  {
    return {};
  }

  if (rows.size() != 1)
  {
    error("Expected to find only one player with name \"" + name + "\"");
  }

  Player out;

  const auto &record = rows[0];
  out.id             = fromDbId(record[0].as<int>());
  out.name           = name;
  out.password       = record[1].as<std::string>();
  out.faction        = fromDbFaction(record[2].as<std::string>());

  return out;
}

auto PlayerRepository::findSystemByPlayer(const Uuid player) const -> Uuid
{
  const auto query = [player](pqxx::nontransaction &work) {
    return work.exec(pqxx::prepped{FIND_SYSTEM_QUERY_NAME}, pqxx::params{toDbId(player)}).one_row();
  };
  const auto record = m_connection->executeQueryReturningSingleRow(query);

  return fromDbId(record[0].as<int>());
}

void PlayerRepository::save(const Player &player)
{
  auto query = [&player](pqxx::work &transaction) {
    return transaction
      .exec(UPDATE_PLAYER_QUERY_NAME,
            pqxx::params{player.name, player.password, toDbFaction(player.faction)})
      .no_rows();
  };

  const auto res = m_connection->tryExecuteTransaction(query);
  if (res.error)
  {
    error("Failed to save player: " + *res.error);
  }
}

} // namespace bsgo
