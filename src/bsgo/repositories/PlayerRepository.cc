
#include "PlayerRepository.hh"

namespace bsgo {

PlayerRepository::PlayerRepository(const DbConnectionShPtr &connection)
  : AbstractRepository("player", connection)
{}

namespace {
constexpr auto FIND_ALL_QUERY_NAME = "player_find_all";
constexpr auto FIND_ALL_QUERY      = "SELECT id FROM system";

constexpr auto FIND_ALL_UNDOCKED_BY_SYSTEM_QUERY_NAME = "system_find_all_undocked_by_system";
constexpr auto FIND_ALL_UNDOCKED_BY_SYSTEM_QUERY      = R"(
SELECT
  ps.player
FROM
  player_ship AS ps
  LEFT JOIN ship_system AS ss ON ps.id = ss.ship
WHERE
  ps.active = true
  AND ss.docked = false
  AND ss.system = $1
)";

constexpr auto FIND_ONE_QUERY_NAME = "player_find_one";
constexpr auto FIND_ONE_QUERY      = R"(
SELECT
  account,
  name,
  faction
FROM
  player
WHERE
  id = $1
)";

constexpr auto FIND_ONE_BY_ACCOUNT_QUERY_NAME = "player_find_one_by_account";
constexpr auto FIND_ONE_BY_ACCOUNT_QUERY      = R"(
SELECT
  id,
  name,
  faction
FROM
  player
WHERE
  account = $1
)";

constexpr auto FIND_SYSTEM_QUERY_NAME = "player_find_system";
constexpr auto FIND_SYSTEM_QUERY      = R"(
SELECT
  ss.system
FROM
  player_ship AS ps
  LEFT JOIN ship_system AS ss ON ps.id = ss.ship
  LEFT JOIN player AS p ON ps.player = p.id
WHERE
  ps.active = true
  AND ps.player IS NOT NULL
  AND ps.player = $1
)";

constexpr auto UPDATE_PLAYER_QUERY_NAME = "player_update";
constexpr auto UPDATE_PLAYER_QUERY      = R"(
INSERT INTO player (account, name, faction)
  VALUES ($1, $2, $3)
)";
} // namespace

void PlayerRepository::initialize()
{
  m_connection->prepare(FIND_ALL_QUERY_NAME, FIND_ALL_QUERY);
  m_connection->prepare(FIND_ALL_UNDOCKED_BY_SYSTEM_QUERY_NAME, FIND_ALL_UNDOCKED_BY_SYSTEM_QUERY);
  m_connection->prepare(FIND_ONE_QUERY_NAME, FIND_ONE_QUERY);
  m_connection->prepare(FIND_ONE_BY_ACCOUNT_QUERY_NAME, FIND_ONE_BY_ACCOUNT_QUERY);
  m_connection->prepare(FIND_SYSTEM_QUERY_NAME, FIND_SYSTEM_QUERY);
  m_connection->prepare(UPDATE_PLAYER_QUERY_NAME, UPDATE_PLAYER_QUERY);
}

auto PlayerRepository::findAll() const -> std::unordered_set<Uuid>
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

auto PlayerRepository::findAllUndockedBySystem(const Uuid system) const -> std::unordered_set<Uuid>
{
  const auto query = [system](pqxx::nontransaction &work) {
    // https://libpqxx.readthedocs.io/stable/parameters.html
    return work.exec(pqxx::prepped{FIND_ALL_UNDOCKED_BY_SYSTEM_QUERY_NAME},
                     pqxx::params{toDbId(system)});
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

  out.id = player;
  if (!record[0].is_null())
  {
    out.account = fromDbId(record[0].as<int>());
  }
  out.name    = record[1].as<std::string>();
  out.faction = fromDbFaction(record[2].as<std::string>());

  return out;
}

auto PlayerRepository::findOneByAccount(const Uuid account) const -> Player
{
  const auto query = [account](pqxx::nontransaction &work) {
    return work.exec(pqxx::prepped{FIND_ONE_BY_ACCOUNT_QUERY_NAME}, pqxx::params{toDbId(account)})
      .one_row();
  };
  const auto record = m_connection->executeQueryReturningSingleRow(query);

  Player out;

  out.id      = fromDbId(record[0].as<int>());
  out.account = account;
  out.name    = record[1].as<std::string>();
  out.faction = fromDbFaction(record[2].as<std::string>());

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
    std::optional<int> maybeAccount{};
    if (player.account)
    {
      maybeAccount = toDbId(*player.account);
    }

    return transaction
      .exec(pqxx::prepped{UPDATE_PLAYER_QUERY_NAME},
            pqxx::params{maybeAccount, player.name, toDbFaction(player.faction)})
      .no_rows();
  };

  const auto res = m_connection->tryExecuteTransaction(query);
  if (res.error)
  {
    error("Failed to save player: " + *res.error);
  }
}

} // namespace bsgo
