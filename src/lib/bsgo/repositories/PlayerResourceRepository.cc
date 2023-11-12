
#include "PlayerResourceRepository.hh"

namespace bsgo {

PlayerResourceRepository::PlayerResourceRepository(const DbConnectionShPtr &connection)
  : AbstractRepository("player", connection)
{
  addModule("resource");
}

namespace {
constexpr auto SQL_QUERY_PLAYER_RESOURCES
  = "SELECT resource.id, resource.name, player_resource.amount FROM player_resource LEFT JOIN resource on player_resource.resource = resource.name WHERE player = ";

auto generateSqlQuery(const Uuid &player) -> std::string
{
  return SQL_QUERY_PLAYER_RESOURCES + str(player + 1);
}
} // namespace

auto PlayerResourceRepository::findAllByPlayer(const Uuid &player) const
  -> std::vector<PlayerResource>
{
  // https://www.tutorialspoint.com/postgresql/pdf/postgresql_c_cpp.pdf
  if (player != Uuid(0))
  {
    error("Player " + str(player) + " not found");
  }

  const auto sql = generateSqlQuery(player);

  pqxx::nontransaction work(m_connection->connection());
  pqxx::result rows(work.exec(sql));

  std::vector<PlayerResource> out;
  for (const auto record : rows)
  {
    const auto resource = record[0].as<Uuid>();
    const auto name     = record[1].as<std::string>();
    const auto amount   = record[2].as<float>();

    out.emplace_back(resource, name, amount);
  }

  return out;
}

} // namespace bsgo
