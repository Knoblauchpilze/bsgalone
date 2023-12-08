
#include "PlayerRepository.hh"

namespace bsgo {

PlayerRepository::PlayerRepository(const DbConnectionShPtr &connection)
  : AbstractRepository("player", connection)
{}

namespace {
constexpr auto SQL_QUERY_NAME = "SELECT id, password, faction FROM player WHERE name = ";
auto generateNameSqlQuery(const std::string &name) -> std::string
{
  return std::string(SQL_QUERY_NAME) + "'" + name + "'";
}

constexpr auto SQL_QUERY_SYSTEM
  = "SELECT ss.system FROM player_ship AS ps LEFT JOIN ship_system AS ss ON ps.id = ss.ship LEFT JOIN player AS p ON ps.player = p.id WHERE ps.active = true AND ps.player IS NOT NULL AND ps.player = ";
auto generateSystemSqlQuery(const Uuid &player) -> std::string
{
  return SQL_QUERY_SYSTEM + std::to_string(toDbId(player));
}

constexpr auto SQL_CREATE_PLAYER_PROCEDURE_NAME = "player_signup";
auto generateSignupSqlQuery(const Player &player) -> std::string
{
  std::string out = "CALL ";
  out += SQL_CREATE_PLAYER_PROCEDURE_NAME;

  out += " (";
  out += "\'" + player.name + "\'";
  out += ",";
  out += "\'" + player.password + "\'";
  out += ",";
  out += "\'" + toDbFaction(player.faction) + "\'";
  out += ")";

  return out;
}
} // namespace

auto PlayerRepository::findOneByName(const std::string &name) const -> std::optional<Player>
{
  const auto sql = generateNameSqlQuery(name);

  pqxx::nontransaction work(m_connection->connection());
  const auto rows(work.exec(sql));

  if (rows.empty())
  {
    return {};
  }

  if (rows.size() != 1)
  {
    error("Expected to find only one player with name \"" + name + "\"");
  }

  Player out;

  const auto &record = rows[0];
  out.id             = fromDbId(record[0].as<int>());
  out.name           = name;
  out.password       = record[1].as<std::string>();
  out.faction        = fromDbFaction(record[2].as<std::string>());

  return out;
}

auto PlayerRepository::findSystemByPlayer(const Uuid &player) const -> Uuid
{
  const auto sql = generateSystemSqlQuery(player);

  pqxx::nontransaction work(m_connection->connection());
  const auto rows(work.exec(sql));

  if (rows.size() != 1)
  {
    error("Expected to find only one system for player with id " + str(player));
  }

  const auto &record = rows[0];
  return fromDbId(record[0].as<int>());
}

auto PlayerRepository::save(const Player &player) -> std::optional<Uuid>
{
  const auto sql = generateSignupSqlQuery(player);

  const auto result = m_connection->safeExecute(sql);
  if (result.error)
  {
    return {};
  }

  const auto dbPlayer = findOneByName(player.name);
  if (!dbPlayer)
  {
    warn("Player not found despite no error during insertion");
    return {};
  }

  return dbPlayer->id;
}

} // namespace bsgo
