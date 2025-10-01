

#include "ComputerRepository.hh"

namespace bsgo {

ComputerRepository::ComputerRepository(const DbConnectionShPtr &connection)
  : AbstractRepository("computer", connection)
{}

namespace {
constexpr auto FIND_ALL_QUERY_NAME = "computer_find_all";
constexpr auto FIND_ALL_QUERY      = "SELECT id FROM computer";

constexpr auto FIND_ONE_QUERY_NAME = "computer_find_one";
constexpr auto FIND_ONE_QUERY      = R"(
SELECT
  name,
  offensive,
  power_cost,
  range,
  reload_time,
  duration,
  damage_modifier
FROM
  computer
WHERE
  id = $1
)";

constexpr auto FIND_ALLOWED_TARGETS_QUERY_NAME = "computer_find_targets";
constexpr auto FIND_ALLOWED_TARGETS_QUERY      = R"(
SELECT
  cat.entity
FROM
  computer_allowed_target AS cat
  LEFT JOIN computer AS c ON cat.computer = c.id
WHERE
  c.id = $1
)";
} // namespace

void ComputerRepository::initialize()
{
  m_connection->prepare(FIND_ALL_QUERY_NAME, FIND_ALL_QUERY);
  m_connection->prepare(FIND_ONE_QUERY_NAME, FIND_ONE_QUERY);
  m_connection->prepare(FIND_ALLOWED_TARGETS_QUERY_NAME, FIND_ALLOWED_TARGETS_QUERY);
}

auto ComputerRepository::findAll() const -> std::unordered_set<Uuid>
{
  const auto query = [](pqxx::nontransaction &work) {
    return work.exec(pqxx::prepped{FIND_ALL_QUERY_NAME});
  };
  const auto rows = m_connection->executeQuery(query);

  std::unordered_set<Uuid> out;
  for (const auto record : rows)
  {
    out.emplace(fromDbId(record[0].as<int>()));
  }

  return out;
}

auto ComputerRepository::findOneById(const Uuid computer) const -> Computer
{
  auto out = fetchComputerBase(computer);
  fetchAllowedTargets(computer, out);

  return out;
}

auto ComputerRepository::fetchComputerBase(const Uuid computer) const -> Computer
{
  const auto query = [computer](pqxx::nontransaction &work) {
    return work.exec(pqxx::prepped{FIND_ONE_QUERY_NAME}, pqxx::params{toDbId(computer)}).one_row();
  };
  const auto record = m_connection->executeQueryReturningSingleRow(query);

  Computer out{};

  out.id        = computer;
  out.name      = record[0].as<std::string>();
  out.offensive = record[1].as<bool>();
  out.powerCost = record[2].as<float>();
  if (!record[3].is_null())
  {
    out.range = {record[3].as<float>()};
  }
  out.reloadTime = Tick::fromInt(record[4].as<int>());
  if (!record[5].is_null())
  {
    out.duration = Tick::fromInt(record[5].as<int>());
  }
  if (!record[6].is_null())
  {
    out.damageModifier = {record[6].as<float>()};
  }

  return out;
}

void ComputerRepository::fetchAllowedTargets(const Uuid computer, Computer &out) const
{
  const auto query = [computer](pqxx::nontransaction &work) {
    return work.exec(pqxx::prepped{FIND_ALLOWED_TARGETS_QUERY_NAME}, pqxx::params{toDbId(computer)});
  };
  const auto rows = m_connection->executeQuery(query);

  std::unordered_set<EntityKind> targets;
  for (const auto record : rows)
  {
    targets.emplace(fromDbEntityKind(record[0].as<std::string>()));
  }

  out.allowedTargets = {targets};
}

} // namespace bsgo
