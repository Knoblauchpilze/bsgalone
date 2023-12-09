
#include "PlayerRepository.hh"

namespace bsgo {

PlayerRepository::PlayerRepository(const DbConnectionShPtr &connection)
  : AbstractRepository("player", connection)
{}

namespace {
constexpr auto FIND_ONE_QUERY_NAME = "player_find_one";
constexpr auto FIND_ONE_QUERY      = "SELECT id, password, faction FROM player WHERE name = $1";

constexpr auto FIND_SYSTEM_QUERY_NAME = "player_find_system";
constexpr auto FIND_SYSTEM_QUERY
  = "SELECT ss.system FROM player_ship AS ps LEFT JOIN ship_system AS ss ON ps.id = ss.ship LEFT JOIN player AS p ON ps.player = p.id WHERE ps.active = true AND ps.player IS NOT NULL AND ps.player = $1";
} // namespace

void PlayerRepository::initialize()
{
  m_connection->prepare(FIND_ONE_QUERY_NAME, FIND_ONE_QUERY);
  m_connection->prepare(FIND_SYSTEM_QUERY_NAME, FIND_SYSTEM_QUERY);
}

auto PlayerRepository::findOneByName(const std::string &name) const -> std::optional<Player>
{
  auto work         = m_connection->nonTransaction();
  const auto record = work.exec_prepared1(FIND_ONE_QUERY_NAME, name);

  Player out;

  out.id       = fromDbId(record[0].as<int>());
  out.name     = name;
  out.password = record[1].as<std::string>();
  out.faction  = fromDbFaction(record[2].as<std::string>());

  return out;
}

auto PlayerRepository::findSystemByPlayer(const Uuid &player) const -> Uuid
{
  auto work         = m_connection->nonTransaction();
  const auto record = work.exec_prepared1(FIND_SYSTEM_QUERY_NAME, toDbId(player));

  return fromDbId(record[0].as<int>());
}

namespace {
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
