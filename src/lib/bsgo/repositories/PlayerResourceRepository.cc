
#include "PlayerResourceRepository.hh"

namespace bsgo {

PlayerResourceRepository::PlayerResourceRepository(const DbConnectionShPtr &connection)
  : AbstractRepository("player", connection)
{
  addModule("resource");
}

namespace {
constexpr auto SQL_QUERY
  = "SELECT resource.id, resource.name, player_resource.amount FROM player_resource LEFT JOIN resource on player_resource.resource = resource.id WHERE player = ";
auto generateSqlQuery(const Uuid &player) -> std::string
{
  return SQL_QUERY + std::to_string(toDbId(player));
}
} // namespace

auto PlayerResourceRepository::findAllByPlayer(const Uuid &player) const
  -> std::vector<PlayerResource>
{
  // https://www.tutorialspoint.com/postgresql/pdf/postgresql_c_cpp.pdf
  const auto sql = generateSqlQuery(player);

  pqxx::nontransaction work(m_connection->connection());
  const auto rows(work.exec(sql));

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
}

void PlayerResourceRepository::save(const PlayerResource &resource)
{
  auto transaction = [&resource](pqxx::connection &connection) {
    // https://libpqxx.readthedocs.io/en/6.4/a01480.html
    connection.prepare(UPDATE_RESOURCE_QUERY_NAME);

    // https://gist.github.com/fearofcode/7516c9b7b18922386148195be5660329
    pqxx::transaction work(connection);

    // https://stackoverflow.com/questions/1109061/insert-on-duplicate-update-in-postgresql
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
