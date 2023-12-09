
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
} // namespace

void PlayerResourceRepository::initialize()
{
  m_connection->prepare(FIND_ALL_QUERY_NAME, FIND_ALL_QUERY);
}

auto PlayerResourceRepository::findAllByPlayer(const Uuid &player) const
  -> std::vector<PlayerResource>
{
  auto work       = m_connection->nonTransaction();
  const auto rows = work.exec_prepared(FIND_ALL_QUERY_NAME, toDbId(player));

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

namespace {
constexpr auto UPDATE_RESOURCE_QUERY_NAME = "update_resource";
constexpr auto UPDATE_RESOURCE_SQL_QUERY  = R"(
INSERT INTO the_table (id, column_1, column_2) 
  VALUES (1, 'A', 'X'), (2, 'B', 'Y'), (3, 'C', 'Z')
  ON CONFLICT (id) DO UPDATE 
  SET column_1 = excluded.column_1, 
      column_2 = excluded.column_2;
)";
} // namespace

void PlayerResourceRepository::save(const PlayerResource &resource)
{
  auto transaction = [&resource](pqxx::connection &connection) {
    // https://libpqxx.readthedocs.io/en/6.4/a01480.html
    // https://stackoverflow.com/questions/1109061/insert-on-duplicate-update-in-postgresql
    connection.prepare(UPDATE_RESOURCE_QUERY_NAME, "select");

    // https://gist.github.com/fearofcode/7516c9b7b18922386148195be5660329
    pqxx::transaction work(connection);

    return work.exec_prepared0(UPDATE_RESOURCE_QUERY_NAME,
                               resource.resource,
                               resource.amount,
                               resource.player);
  };

  const auto res = m_connection->safeExecute(transaction);
  if (res.error)
  {
    error("Failed to save resources: " + *res.error);
  }
}

} // namespace bsgo
