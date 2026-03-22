
#include "PlayerRepository.hh"

namespace bsgalone::core {

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

constexpr auto FIND_ALL_BY_SYSTEM_QUERY_NAME = "system_find_all_by_system";
constexpr auto FIND_ALL_BY_SYSTEM_QUERY      = R"(
SELECT
  ps.player
FROM
  player_ship AS ps
  LEFT JOIN ship_system AS ss ON ps.id = ss.ship
WHERE
  ps.active = true
  AND ss.system = $1
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
  AND ps.player = $1
)";

constexpr auto FIND_ONE_QUERY_NAME = "player_find_one";
constexpr auto FIND_ONE_QUERY      = R"(
SELECT
  p.id,
  p.account,
  p.name,
  p.faction,
  pr.role
FROM
  player AS p
  INNER JOIN player_role AS pr ON pr.player = p.id
WHERE
  p.id = $1
)";

constexpr auto FIND_ONE_BY_ACCOUNT_QUERY_NAME = "player_find_one_by_account";
constexpr auto FIND_ONE_BY_ACCOUNT_QUERY      = R"(
SELECT
  p.id,
  p.account,
  p.name,
  p.faction,
  pr.role
FROM
  player AS p
  INNER JOIN player_role AS pr ON pr.player = p.id
WHERE
  p.account = $1
)";

constexpr auto UPDATE_PLAYER_QUERY_NAME = "player_update";
constexpr auto UPDATE_PLAYER_QUERY      = R"(
INSERT INTO player (account, name, faction)
  VALUES ($1, $2, $3)
  ON CONFLICT (account) DO UPDATE
  SET
    name = excluded.name
  RETURNING id
)";

constexpr auto UPDATE_PLAYER_ROLE_QUERY_NAME = "player_role_update";
constexpr auto UPDATE_PLAYER_ROLE_QUERY      = R"(
INSERT INTO player_role (player, role)
  VALUES ($1, $2)
  ON CONFLICT (player) DO UPDATE
  SET
    role = excluded.role
)";
} // namespace

void PlayerRepository::initialize()
{
  m_connection->prepare(FIND_ALL_QUERY_NAME, FIND_ALL_QUERY);
  m_connection->prepare(FIND_ALL_BY_SYSTEM_QUERY_NAME, FIND_ALL_BY_SYSTEM_QUERY);
  m_connection->prepare(FIND_ALL_UNDOCKED_BY_SYSTEM_QUERY_NAME, FIND_ALL_UNDOCKED_BY_SYSTEM_QUERY);
  m_connection->prepare(FIND_SYSTEM_QUERY_NAME, FIND_SYSTEM_QUERY);
  m_connection->prepare(FIND_ONE_QUERY_NAME, FIND_ONE_QUERY);
  m_connection->prepare(FIND_ONE_BY_ACCOUNT_QUERY_NAME, FIND_ONE_BY_ACCOUNT_QUERY);
  m_connection->prepare(UPDATE_PLAYER_QUERY_NAME, UPDATE_PLAYER_QUERY);
  m_connection->prepare(UPDATE_PLAYER_ROLE_QUERY_NAME, UPDATE_PLAYER_ROLE_QUERY);
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

auto PlayerRepository::findAllBySystem(const Uuid system) const -> std::unordered_set<Uuid>
{
  const auto query = [system](pqxx::nontransaction &work) {
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

auto PlayerRepository::findSystemByPlayer(const Uuid player) const -> Uuid
{
  const auto query = [player](pqxx::nontransaction &work) {
    return work.exec(pqxx::prepped{FIND_SYSTEM_QUERY_NAME}, pqxx::params{toDbId(player)}).one_row();
  };
  const auto record = m_connection->executeQueryReturningSingleRow(query);

  return fromDbId(record[0].as<int>());
}

namespace {
auto fromDbRow(const pqxx::row &record) -> Player
{
  std::optional<Uuid> maybeAccountDbId{};
  if (!record[1].is_null())
  {
    maybeAccountDbId = fromDbId(record[1].as<int>());
  }

  return Player{
    .dbId    = fromDbId(record[0].as<int>()),
    .account = maybeAccountDbId,
    .name    = record[2].as<std::string>(),
    .faction = fromDbFaction(record[3].as<std::string>()),
    .role    = fromDbGameRole(record[4].as<std::string>()),
  };
}
} // namespace

auto PlayerRepository::findOneById(const Uuid playerDbId) const -> Player
{
  const auto query = [playerDbId](pqxx::nontransaction &work) {
    return work.exec(pqxx::prepped{FIND_ONE_QUERY_NAME}, pqxx::params{toDbId(playerDbId)}).one_row();
  };
  const auto record = m_connection->executeQueryReturningSingleRow(query);

  return fromDbRow(record);
}

auto PlayerRepository::findOneByAccount(const Uuid accountDbId) const -> Player
{
  const auto query = [accountDbId](pqxx::nontransaction &work) {
    return work
      .exec(pqxx::prepped{FIND_ONE_BY_ACCOUNT_QUERY_NAME}, pqxx::params{toDbId(accountDbId)})
      .one_row();
  };
  const auto record = m_connection->executeQueryReturningSingleRow(query);

  return fromDbRow(record);
}

auto PlayerRepository::save(Player player) -> Player
{
  auto playerQuery = [&player](pqxx::nontransaction &work) {
    std::optional<int> maybeAccount{};
    if (player.account)
    {
      maybeAccount = toDbId(*player.account);
    }

    return work
      .exec(pqxx::prepped{UPDATE_PLAYER_QUERY_NAME},
            pqxx::params{maybeAccount, player.name, toDbFaction(player.faction)})
      .one_row();
  };

  const auto record = m_connection->executeQueryReturningSingleRow(playerQuery);
  player.dbId       = fromDbId(record[0].as<int>());

  auto roleQuery = [&player](pqxx::work &transaction) {
    return transaction
      .exec(pqxx::prepped{UPDATE_PLAYER_ROLE_QUERY_NAME},
            pqxx::params{toDbId(player.dbId), toDbGameRole(player.role)})
      .no_rows();
  };

  const auto res = m_connection->tryExecuteTransaction(roleQuery);
  if (res.error)
  {
    error("Failed to save player role: " + *res.error);
  }

  return player;
}

} // namespace bsgalone::core
