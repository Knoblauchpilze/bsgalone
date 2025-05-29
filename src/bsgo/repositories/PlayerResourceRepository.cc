
#include "PlayerResourceRepository.hh"

namespace bsgo {

PlayerResourceRepository::PlayerResourceRepository(const DbConnectionShPtr &connection)
  : AbstractRepository("player", connection)
{
  addModule("resource");
}

namespace {
constexpr auto FIND_ALL_QUERY_NAME = "player_resource_find_all";
constexpr auto FIND_ALL_QUERY
  = "SELECT resource.id, resource.name, player_resource.amount FROM player_resource LEFT JOIN resource on player_resource.resource = resource.id WHERE player = $1";

constexpr auto UPDATE_RESOURCE_QUERY_NAME = "player_resource_update";
// https://stackoverflow.com/questions/1109061/insert-on-duplicate-update-in-postgresql
constexpr auto UPDATE_RESOURCE_QUERY = R"(
INSERT INTO player_resource (player, resource, amount)
  VALUES ($1, $2, $3)
  ON CONFLICT (player, resource) DO UPDATE
  SET
    amount = excluded.amount
  WHERE
    player_resource.player = excluded.player
    AND player_resource.resource = excluded.resource
)";
} // namespace

void PlayerResourceRepository::initialize()
{
  m_connection->prepare(FIND_ALL_QUERY_NAME, FIND_ALL_QUERY);
  m_connection->prepare(UPDATE_RESOURCE_QUERY_NAME, UPDATE_RESOURCE_QUERY);
}

auto PlayerResourceRepository::findAllByPlayer(const Uuid player) const
  -> std::vector<PlayerResource>
{
  const auto query = [player](pqxx::nontransaction &work) {
    return work.exec(FIND_ALL_QUERY_NAME, pqxx::params{toDbId(player)});
  };
  const auto rows = m_connection->executeQuery(query);

  std::vector<PlayerResource> out;
  for (const auto record : rows)
  {
    const auto resource = fromDbId(record[0].as<int>());
    const auto name     = record[1].as<std::string>();
    const auto amount   = record[2].as<float>();

    out.emplace_back(player, resource, name, amount);
  }

  return out;
}

void PlayerResourceRepository::save(const PlayerResource &resource)
{
  // https://gist.github.com/fearofcode/7516c9b7b18922386148195be5660329#file-libpqxx_test-cpp-L24
  auto query = [&resource](pqxx::work &transaction) {
    return transaction
      .exec(UPDATE_RESOURCE_QUERY_NAME,
            pqxx::params{toDbId(resource.player), toDbId(resource.resource), resource.amount})
      .no_rows();
  };

  const auto res = m_connection->tryExecuteTransaction(query);
  if (res.error)
  {
    error("Failed to save resources: " + *res.error);
  }
}

} // namespace bsgo
