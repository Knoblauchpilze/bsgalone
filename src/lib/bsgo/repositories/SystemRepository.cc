
#include "SystemRepository.hh"

namespace bsgo {

SystemRepository::SystemRepository(const DbConnectionShPtr &connection)
  : AbstractRepository("system", connection)
{}

namespace {
constexpr auto FIND_ALL_QUERY_NAME = "system_find_all";
constexpr auto FIND_ALL_QUERY      = "SELECT id FROM system";

constexpr auto FIND_ONE_QUERY_NAME = "system_find_one";
constexpr auto FIND_ONE_QUERY      = "SELECT name, x_pos, y_pos, z_pos FROM system WHERE id = $1";

constexpr auto FIND_ONE_BY_FACTION_QUERY_NAME = "system_find_one_by_faction";
constexpr auto FIND_ONE_BY_FACTION_QUERY = "SELECT system FROM starting_system WHERE faction = $1";

constexpr auto FIND_ASTEROIDS_QUERY_NAME = "system_find_asteroids";
constexpr auto FIND_ASTEROIDS_QUERY      = "SELECT id FROM asteroid WHERE system = $1";

constexpr auto FIND_SHIPS_QUERY_NAME = "system_find_ships";
constexpr auto FIND_SHIPS_QUERY      = "SELECT ship FROM ship_system WHERE system = $1";

constexpr auto FIND_OUTPOSTS_QUERY_NAME = "system_find_outposts";
constexpr auto FIND_OUTPOSTS_QUERY      = "SELECT id FROM system_outpost WHERE system = $1";

constexpr auto UPDATE_SYSTEM_QUERY_NAME = "system_update_ship";
constexpr auto UPDATE_SYSTEM_QUERY      = R"(
INSERT INTO ship_system (ship, system)
  VALUES ($1, $2)
  ON CONFLICT (ship, system) DO UPDATE
  SET
    system = excluded.system
  WHERE
    ship_system.ship = excluded.ship
)";
} // namespace

void SystemRepository::initialize()
{
  m_connection->prepare(FIND_ALL_QUERY_NAME, FIND_ALL_QUERY);
  m_connection->prepare(FIND_ONE_QUERY_NAME, FIND_ONE_QUERY);
  m_connection->prepare(FIND_ONE_BY_FACTION_QUERY_NAME, FIND_ONE_BY_FACTION_QUERY);
  m_connection->prepare(FIND_ASTEROIDS_QUERY_NAME, FIND_ASTEROIDS_QUERY);
  m_connection->prepare(FIND_SHIPS_QUERY_NAME, FIND_SHIPS_QUERY);
  m_connection->prepare(FIND_OUTPOSTS_QUERY_NAME, FIND_OUTPOSTS_QUERY);
  m_connection->prepare(UPDATE_SYSTEM_QUERY_NAME, UPDATE_SYSTEM_QUERY);
}

auto SystemRepository::findAll() const -> std::unordered_set<Uuid>
{
  auto work       = m_connection->nonTransaction();
  const auto rows = work.exec_prepared(FIND_ALL_QUERY_NAME);

  std::unordered_set<Uuid> out;
  for (const auto record : rows)
  {
    out.emplace(fromDbId(record[0].as<int>()));
  }

  return out;
}

auto SystemRepository::findOneById(const Uuid &system) const -> System
{
  auto work         = m_connection->nonTransaction();
  const auto record = work.exec_prepared1(FIND_ONE_QUERY_NAME, toDbId(system));

  System out{};

  out.id   = system;
  out.name = record[0].as<std::string>();

  const auto x = record[1].as<float>();
  const auto y = record[2].as<float>();
  const auto z = record[3].as<float>();
  out.position = Eigen::Vector3f(x, y, z);

  return out;
}

auto SystemRepository::findOneByFactionAndStarting(const Faction &faction) const -> Uuid
{
  auto work         = m_connection->nonTransaction();
  const auto record = work.exec_prepared1(FIND_ONE_BY_FACTION_QUERY_NAME, toDbFaction(faction));

  return fromDbId(record[0].as<int>());
}

auto SystemRepository::findAllAsteroidsBySystem(const Uuid &system) const
  -> std::unordered_set<Uuid>
{
  auto work       = m_connection->nonTransaction();
  const auto rows = work.exec_prepared(FIND_ASTEROIDS_QUERY_NAME, toDbId(system));

  std::unordered_set<Uuid> out;
  for (const auto record : rows)
  {
    out.emplace(fromDbId(record[0].as<int>()));
  }

  return out;
}

auto SystemRepository::findAllShipsBySystem(const Uuid &system) const -> std::unordered_set<Uuid>
{
  auto work       = m_connection->nonTransaction();
  const auto rows = work.exec_prepared(FIND_SHIPS_QUERY_NAME, toDbId(system));

  std::unordered_set<Uuid> out;
  for (const auto record : rows)
  {
    out.emplace(fromDbId(record[0].as<int>()));
  }

  return out;
}

auto SystemRepository::findAllOutpostsBySystem(const Uuid &system) const -> std::unordered_set<Uuid>
{
  auto work       = m_connection->nonTransaction();
  const auto rows = work.exec_prepared(FIND_OUTPOSTS_QUERY_NAME, toDbId(system));

  std::unordered_set<Uuid> out;
  for (const auto record : rows)
  {
    out.emplace(fromDbId(record[0].as<int>()));
  }

  return out;
}

void SystemRepository::updateSystemForShip(const Uuid &ship, const Uuid &system)
{
  auto query = [&ship, &system](pqxx::work &transaction) {
    return transaction.exec_prepared0(UPDATE_SYSTEM_QUERY_NAME, toDbId(ship), toDbId(system));
  };

  const auto res = m_connection->tryExecuteTransaction(query);
  if (res.error)
  {
    error("Failed to update system for ship: " + *res.error);
  }
}

} // namespace bsgo
