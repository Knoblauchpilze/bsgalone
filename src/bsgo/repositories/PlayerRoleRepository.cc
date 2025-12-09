
#include "PlayerRoleRepository.hh"

namespace bsgo {

PlayerRoleRepository::PlayerRoleRepository(const DbConnectionShPtr &connection)
  : AbstractRepository("player", connection)
{
  addModule("role");
}

namespace {
constexpr auto FIND_ONE_BY_PLAYER_QUERY_NAME = "player_role_find_one_by_player";
constexpr auto FIND_ONE_BY_PLAYER_QUERY      = R"(
SELECT
  role,
  target_ship
FROM
  player_role
WHERE
  player = $1
)";

constexpr auto UPDATE_PLAYER_ROLE_QUERY_NAME = "player_role_update";
constexpr auto UPDATE_PLAYER_ROLE_QUERY      = R"(
INSERT INTO player_role (player, role, target_ship)
  VALUES ($1, $2, $3)
  ON CONFLICT (player) DO UPDATE
  SET
    role = excluded.role,
    target_ship = excluded.target_ship
  WHERE
    player_role.player = excluded.player
    AND player_role.player = excluded.player
)";
} // namespace

void PlayerRoleRepository::initialize()
{
  m_connection->prepare(FIND_ONE_BY_PLAYER_QUERY_NAME, FIND_ONE_BY_PLAYER_QUERY);
  m_connection->prepare(UPDATE_PLAYER_ROLE_QUERY_NAME, UPDATE_PLAYER_ROLE_QUERY);
}

auto PlayerRoleRepository::findOneByPlayer(const Uuid player) const -> PlayerRole
{
  const auto query = [player](pqxx::nontransaction &work) {
    return work.exec(pqxx::prepped{FIND_ONE_BY_PLAYER_QUERY_NAME}, pqxx::params{toDbId(player)})
      .one_row();
  };
  const auto record = m_connection->executeQueryReturningSingleRow(query);

  PlayerRole out;

  out.player = player;
  out.role   = fromDbGameRole(record[0].as<std::string>());
  if (!record[1].is_null())
  {
    out.targetShip = fromDbId(record[1].as<int>());
  }

  return out;
}

void PlayerRoleRepository::save(const PlayerRole &playerRole)
{
  std::optional<int> maybeTargetShip{};
  if (playerRole.targetShip)
  {
    maybeTargetShip = toDbId(*playerRole.targetShip);
  }

  auto query = [&playerRole, maybeTargetShip](pqxx::work &transaction) {
    return transaction
      .exec(pqxx::prepped{UPDATE_PLAYER_ROLE_QUERY_NAME},
            pqxx::params{toDbId(playerRole.player), toDbGameRole(playerRole.role), maybeTargetShip})
      .no_rows();
  };

  const auto res = m_connection->tryExecuteTransaction(query);
  if (res.error)
  {
    error("Failed to save player role: " + *res.error);
  }
}

} // namespace bsgo
