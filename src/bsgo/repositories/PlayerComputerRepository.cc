
#include "PlayerComputerRepository.hh"

namespace bsgo {

PlayerComputerRepository::PlayerComputerRepository(const DbConnectionShPtr &connection)
  : AbstractRepository("computer", connection)
{
  addModule("player");
}

namespace {
constexpr auto FIND_ALL_QUERY_NAME = "player_computer_find_all";
constexpr auto FIND_ALL_QUERY      = "SELECT id FROM player_computer WHERE player = $1";

constexpr auto FIND_ONE_QUERY_NAME = "player_computer_find_one";
constexpr auto FIND_ONE_QUERY
  = "SELECT pc.computer, pc.player, c.name, c.offensive, c.power_cost, c.range, c.reload_time_ms, c.duration_ms, c.damage_modifier, pc.level FROM player_computer AS pc LEFT JOIN computer AS c ON pc.computer = c.id WHERE pc.id = $1";

constexpr auto FIND_ALLOWED_TARGETS_QUERY_NAME = "player_computer_find_targets";
constexpr auto FIND_ALLOWED_TARGETS_QUERY
  = "SELECT cat.entity FROM computer_allowed_target AS cat LEFT JOIN computer AS c ON cat.computer = c.id LEFT JOIN player_computer AS pc ON pc.computer = c.id WHERE pc.id = $1";

constexpr auto UPDATE_COMPUTER_QUERY_NAME = "player_computer_update";
constexpr auto UPDATE_COMPUTER_QUERY      = R"(
INSERT INTO player_computer (computer, player, level)
  VALUES ($1, $2, 1)
)";
} // namespace

void PlayerComputerRepository::initialize()
{
  m_connection->prepare(FIND_ALL_QUERY_NAME, FIND_ALL_QUERY);
  m_connection->prepare(FIND_ONE_QUERY_NAME, FIND_ONE_QUERY);
  m_connection->prepare(FIND_ALLOWED_TARGETS_QUERY_NAME, FIND_ALLOWED_TARGETS_QUERY);
  m_connection->prepare(UPDATE_COMPUTER_QUERY_NAME, UPDATE_COMPUTER_QUERY);
}

auto PlayerComputerRepository::findOneById(const Uuid computer) const -> PlayerComputer
{
  auto out = fetchComputerBase(computer);
  fetchAllowedTargets(computer, out);

  return out;
}

auto PlayerComputerRepository::findAllByPlayer(const Uuid player) const -> std::unordered_set<Uuid>
{
  const auto query = [player](pqxx::nontransaction &work) {
    return work.exec_prepared(FIND_ALL_QUERY_NAME, toDbId(player));
  };
  const auto rows = m_connection->executeQuery(query);

  std::unordered_set<Uuid> out;
  for (const auto record : rows)
  {
    out.emplace(fromDbId(record[0].as<int>()));
  }

  return out;
}

void PlayerComputerRepository::save(const PlayerComputer &computer)
{
  auto query = [&computer](pqxx::work &transaction) {
    return transaction.exec_prepared0(UPDATE_COMPUTER_QUERY_NAME,
                                      toDbId(computer.computer),
                                      toDbId(computer.player));
  };

  const auto res = m_connection->tryExecuteTransaction(query);
  if (res.error)
  {
    error("Failed to save player computer: " + *res.error);
  }
}

auto PlayerComputerRepository::fetchComputerBase(const Uuid computer) const -> PlayerComputer
{
  const auto query = [computer](pqxx::nontransaction &work) {
    return work.exec_prepared1(FIND_ONE_QUERY_NAME, toDbId(computer));
  };
  const auto record = m_connection->executeQueryReturningSingleRow(query);

  PlayerComputer out{};

  out.id        = computer;
  out.computer  = fromDbId(record[0].as<int>());
  out.player    = fromDbId(record[1].as<int>());
  out.name      = record[2].as<std::string>();
  out.offensive = record[3].as<bool>();
  out.powerCost = record[4].as<float>();
  if (!record[5].is_null())
  {
    out.range = {record[5].as<float>()};
  }
  out.reloadTime = core::Milliseconds(record[6].as<int>());
  if (!record[7].is_null())
  {
    out.duration = {core::Milliseconds(record[7].as<int>())};
  }
  if (!record[8].is_null())
  {
    out.damageModifier = {record[8].as<float>()};
  }
  out.level = record[9].as<int>();

  return out;
}

void PlayerComputerRepository::fetchAllowedTargets(const Uuid computer, PlayerComputer &out) const
{
  const auto query = [computer](pqxx::nontransaction &work) {
    return work.exec_prepared(FIND_ALLOWED_TARGETS_QUERY_NAME, toDbId(computer));
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
