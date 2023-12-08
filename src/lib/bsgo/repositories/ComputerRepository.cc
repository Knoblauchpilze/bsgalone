

#include "ComputerRepository.hh"

namespace bsgo {

ComputerRepository::ComputerRepository(const DbConnectionShPtr &connection)
  : AbstractRepository("computer", connection)
{}

namespace {
constexpr auto SQL_QUERY_ALL = "SELECT id FROM computer";

constexpr auto SQL_QUERY
  = "SELECT name, offensive, power_cost, range, reload_time_ms, duration_ms, damage_modifier FROM computer WHERE id = ";
auto generateSqlQuery(const Uuid &computer) -> std::string
{
  return SQL_QUERY + std::to_string(toDbId(computer));
}

constexpr auto SQL_QUERY_TARGET
  = "SELECT cat.entity FROM computer_allowed_target AS cat LEFT JOIN computer AS c ON cat.computer = c.id WHERE c.id = ";
auto generateTargetsSqlQuery(const Uuid &computer) -> std::string
{
  return SQL_QUERY_TARGET + std::to_string(toDbId(computer));
}
} // namespace

auto ComputerRepository::findAll() const -> std::unordered_set<Uuid>
{
  pqxx::nontransaction work(m_connection->connection());
  const auto rows(work.exec(SQL_QUERY_ALL));

  std::unordered_set<Uuid> out;
  for (const auto record : rows)
  {
    out.emplace(fromDbId(record[0].as<int>()));
  }

  return out;
}

auto ComputerRepository::findOneById(const Uuid &computer) const -> Computer
{
  auto out = fetchComputerBase(computer);
  fetchAllowedTargets(computer, out);

  return out;
}

auto ComputerRepository::fetchComputerBase(const Uuid &computer) const -> Computer
{
  const auto sql = generateSqlQuery(computer);

  pqxx::nontransaction work(m_connection->connection());
  const auto rows(work.exec(sql));

  if (rows.size() != 1)
  {
    error("Expected to find only one computer with id " + str(computer));
  }

  Computer out{};

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

  return out;
}

void ComputerRepository::fetchAllowedTargets(const Uuid &computer, Computer &out) const
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
