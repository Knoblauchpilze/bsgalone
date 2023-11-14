
#include "PlayerRepository.hh"

namespace bsgo {

PlayerRepository::PlayerRepository(const DbConnectionShPtr &connection)
  : AbstractRepository("player", connection)
{}

namespace {
constexpr auto SQL_QUERY = "SELECT system FROM player_system WHERE player = ";
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
