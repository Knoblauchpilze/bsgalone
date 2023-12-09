
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
  = "SELECT c.name, c.offensive, c.power_cost, c.range, c.reload_time_ms, c.duration_ms, c.damage_modifier, pc.level FROM player_computer AS pc LEFT JOIN computer AS c ON pc.computer = c.id WHERE pc.id = $1";

constexpr auto FIND_ALLOWED_TARGETS_QUERY_NAME = "player_computer_find_targets";
constexpr auto FIND_ALLOWED_TARGETS_QUERY
  = "SELECT cat.entity FROM computer_allowed_target AS cat LEFT JOIN computer AS c ON cat.computer = c.id LEFT JOIN player_computer AS pc ON pc.computer = c.id WHERE pc.id = $1";
} // namespace

void PlayerComputerRepository::initialize()
{
  m_connection->prepare(FIND_ALL_QUERY_NAME, FIND_ALL_QUERY);
  m_connection->prepare(FIND_ONE_QUERY_NAME, FIND_ONE_QUERY);
  m_connection->prepare(FIND_ALLOWED_TARGETS_QUERY_NAME, FIND_ALLOWED_TARGETS_QUERY);
}

auto PlayerComputerRepository::findOneById(const Uuid &computer) const -> PlayerComputer
{
  auto out = fetchComputerBase(computer);
  fetchAllowedTargets(computer, out);

  return out;
}

auto PlayerComputerRepository::findAllByPlayer(const Uuid &player) const -> std::unordered_set<Uuid>
{
  auto work       = m_connection->nonTransaction();
  const auto rows = work.exec_prepared(FIND_ALL_QUERY_NAME, toDbId(player));

  std::unordered_set<Uuid> out;
  for (const auto record : rows)
  {
    out.emplace(fromDbId(record[0].as<int>()));
  }

  return out;
}

void PlayerComputerRepository::save(const PlayerComputer & /*computer*/) {}

auto PlayerComputerRepository::fetchComputerBase(const Uuid &computer) const -> PlayerComputer
{
  auto work         = m_connection->nonTransaction();
  const auto record = work.exec_prepared1(FIND_ONE_QUERY_NAME, toDbId(computer));

  PlayerComputer out{};

  out.id        = computer;
  out.name      = record[0].as<std::string>();
  out.offensive = record[1].as<bool>();
  out.powerCost = record[2].as<float>();
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
  auto work       = m_connection->nonTransaction();
  const auto rows = work.exec_prepared(FIND_ALLOWED_TARGETS_QUERY_NAME, toDbId(computer));

  std::unordered_set<EntityKind> targets;
  for (const auto record : rows)
  {
    targets.emplace(fromDbEntityKind(record[0].as<std::string>()));
  }

  out.allowedTargets = {targets};
}

} // namespace bsgo
