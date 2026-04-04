
#include "PlayerRepository.hh"

namespace bsgalone::core {

PlayerRepository::PlayerRepository(DbConnectionShPtr connection)
  : AbstractRepository("player", std::move(connection))
{}

namespace {
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

constexpr auto UPDATE_PLAYER_ROLE_QUERY_NAME = "player_update_role";
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
  m_connection->prepare(FIND_ONE_QUERY_NAME, FIND_ONE_QUERY);
  m_connection->prepare(FIND_ONE_BY_ACCOUNT_QUERY_NAME, FIND_ONE_BY_ACCOUNT_QUERY);
  m_connection->prepare(UPDATE_PLAYER_QUERY_NAME, UPDATE_PLAYER_QUERY);
  m_connection->prepare(UPDATE_PLAYER_ROLE_QUERY_NAME, UPDATE_PLAYER_ROLE_QUERY);
}

namespace {
auto buildPlayerFromDbRow(const pqxx::row &record) -> Player
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

  return buildPlayerFromDbRow(record);
}

auto PlayerRepository::findOneByAccount(const Uuid accountDbId) const -> Player
{
  const auto query = [accountDbId](pqxx::nontransaction &work) {
    return work
      .exec(pqxx::prepped{FIND_ONE_BY_ACCOUNT_QUERY_NAME}, pqxx::params{toDbId(accountDbId)})
      .one_row();
  };
  const auto record = m_connection->executeQueryReturningSingleRow(query);

  return buildPlayerFromDbRow(record);
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
