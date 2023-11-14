
#include "PlayerRepository.hh"

namespace bsgo {

PlayerRepository::PlayerRepository(const DbConnectionShPtr &connection)
  : AbstractRepository("player", connection)
{}

namespace {
constexpr auto SQL_QUERY
  = "SELECT ss.system FROM player_ship AS ps LEFT JOIN ship_system AS ss ON ps.ship = ss.ship LEFT JOIN player AS p ON ps.player = p.id WHERE ps.active = true AND ps.player IS NOT NULL AND ps.player = ";
auto generateSqlQuery(const Uuid &player) -> std::string
{
  return SQL_QUERY + std::to_string(toDbId(player));
}
} // namespace

auto PlayerRepository::findSystemByPlayer(const Uuid &player) const -> Uuid
{
  const auto sql = generateSqlQuery(player);

  pqxx::nontransaction work(m_connection->connection());
  pqxx::result rows(work.exec(sql));

  if (rows.size() != 1)
  {
    error("Expected to find only one system for player with id " + str(player));
  }

  const auto &record = rows[0];
  return fromDbId(record[0].as<int>());
}

} // namespace bsgo
