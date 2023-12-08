
#include "PlayerComputerRepository.hh"

namespace bsgo {

PlayerComputerRepository::PlayerComputerRepository(const DbConnectionShPtr &connection)
  : AbstractRepository("computer", connection)
{
  addModule("player");
}

namespace {
constexpr auto SQL_QUERY_ALL = "SELECT id FROM player_computer WHERE player = ";
auto generateAllSqlQuery(const Uuid &player) -> std::string
{
  return SQL_QUERY_ALL + std::to_string(toDbId(player));
}

constexpr auto SQL_QUERY
  = "SELECT c.name, c.offensive, c.power_cost, c.range, c.reload_time_ms, c.duration_ms, c.damage_modifier, pc.level FROM player_computer AS pc LEFT JOIN computer AS c ON pc.computer = c.id WHERE pc.id = ";
auto generateSqlQuery(const Uuid &computer) -> std::string
{
  return SQL_QUERY + std::to_string(toDbId(computer));
}

constexpr auto SQL_QUERY_TARGET
  = "SELECT cat.entity FROM computer_allowed_target AS cat LEFT JOIN computer AS c ON cat.computer = c.id LEFT JOIN player_computer AS pc ON pc.computer = c.id WHERE pc.id = ";

auto generateTargetsSqlQuery(const Uuid &computer) -> std::string
{
  return SQL_QUERY_TARGET + std::to_string(toDbId(computer));
}
} // namespace

auto PlayerComputerRepository::findOneById(const Uuid &computer) const -> PlayerComputer
{
  auto out = fetchComputerBase(computer);
  fetchAllowedTargets(computer, out);

  return out;
}

auto PlayerComputerRepository::findAllByPlayer(const Uuid &player) const -> std::unordered_set<Uuid>
{
  const auto sql = generateAllSqlQuery(player);

  pqxx::nontransaction work(m_connection->connection());
  const auto rows(work.exec(sql));

  std::unordered_set<Uuid> out;
  for (const auto record : rows)
  {
    out.emplace(fromDbId(record[0].as<int>()));
  }

  return out;
}

auto PlayerComputerRepository::fetchComputerBase(const Uuid &computer) const -> PlayerComputer
{
  const auto sql = generateSqlQuery(computer);

  pqxx::nontransaction work(m_connection->connection());
  const auto rows(work.exec(sql));

  if (rows.size() != 1)
  {
    error("Expected to find only one computer with id " + str(computer));
  }

  PlayerComputer out{};

  const auto &record = rows[0];
  out.id             = computer;
  out.name           = record[0].as<std::string>();
  out.offensive      = record[1].as<bool>();
  out.powerCost      = record[2].as<float>();
  if (!record[3].is_null())
  {
    out.range = {record[3].as<float>()};
  }
  out.reloadTime = utils::Milliseconds(record[4].as<int>());
  if (!record[5].is_null())
  {
    out.duration = {utils::Milliseconds(record[5].as<int>())};
  }
  if (!record[6].is_null())
  {
    out.damageModifier = {record[6].as<float>()};
  }
  out.level = record[7].as<int>();

  return out;
}

void PlayerComputerRepository::fetchAllowedTargets(const Uuid &computer, PlayerComputer &out) const
{
  const auto sql = generateTargetsSqlQuery(computer);

  pqxx::nontransaction work(m_connection->connection());
  const auto rows(work.exec(sql));

  std::unordered_set<EntityKind> targets;
  for (const auto record : rows)
  {
    targets.emplace(fromDbEntityKind(record[0].as<std::string>()));
  }

  out.allowedTargets = {targets};
}

} // namespace bsgo
