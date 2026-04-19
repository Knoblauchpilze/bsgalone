
#include "SystemRepository.hh"

namespace bsgalone::server {

SystemRepository::SystemRepository(const DbConnectionShPtr &connection)
  : AbstractRepository("system", connection)
{}

namespace {

constexpr auto FIND_ONE_QUERY_NAME = "system_find_one";
constexpr auto FIND_ONE_QUERY      = R"(
SELECT
  s.id,
  s.name,
  s.x_pos,
  s.y_pos,
  s.z_pos,
  t.current_tick,
  tc.ticks,
  tc.duration,
  tc.unit
FROM
  system AS s
  INNER JOIN tick AS t ON t.system = s.id
  INNER JOIN tick_config AS tc ON tc.system = s.id
WHERE
  s.id = $1
)";

constexpr auto FIND_ALL_QUERY_NAME = "system_find_all";
constexpr auto FIND_ALL_QUERY      = R"(
SELECT
  s.id,
  s.name,
  s.x_pos,
  s.y_pos,
  s.z_pos,
  t.current_tick,
  tc.ticks,
  tc.duration,
  tc.unit
FROM
  system AS s
  INNER JOIN tick AS t ON t.system = s.id
  INNER JOIN tick_config AS tc ON tc.system = s.id
)";

constexpr auto UPDATE_SYSTEM_QUERY_NAME = "system_insert";
constexpr auto UPDATE_SYSTEM_QUERY      = R"(
INSERT INTO system (id, name, x_pos, y_pos, z_pos)
  VALUES ($1, $2, $3, $4, $5)
  ON CONFLICT (id) DO UPDATE
  SET
    x_pos = excluded.x_pos,
    y_pos = excluded.y_pos,
    z_pos = excluded.z_pos
)";

constexpr auto UPDATE_SYSTEM_TICK_QUERY_NAME = "system_update_tick";
constexpr auto UPDATE_SYSTEM_TICK_QUERY      = R"(
INSERT INTO tick (system, current_tick)
  VALUES ($1, $2)
  ON CONFLICT (system) DO UPDATE
  SET
    current_tick = excluded.current_tick
)";

constexpr auto UPDATE_SYSTEM_TICK_CONFIG_QUERY_NAME = "system_update_tick_config";
constexpr auto UPDATE_SYSTEM_TICK_CONFIG_QUERY      = R"(
INSERT INTO tick_config (system, duration, unit, ticks)
  VALUES ($1, $2, $3, $4)
  ON CONFLICT (system) DO UPDATE
  SET
    duration = excluded.duration,
    unit = excluded.unit,
    ticks = excluded.ticks
)";
} // namespace

void SystemRepository::initialize()
{
  m_connection->prepare(FIND_ALL_QUERY_NAME, FIND_ALL_QUERY);
  m_connection->prepare(FIND_ONE_QUERY_NAME, FIND_ONE_QUERY);
  m_connection->prepare(UPDATE_SYSTEM_QUERY_NAME, UPDATE_SYSTEM_QUERY);
  m_connection->prepare(UPDATE_SYSTEM_TICK_QUERY_NAME, UPDATE_SYSTEM_TICK_QUERY);
  m_connection->prepare(UPDATE_SYSTEM_TICK_CONFIG_QUERY_NAME, UPDATE_SYSTEM_TICK_CONFIG_QUERY);
}

namespace {
auto fromDbRow(const pqxx::row &record) -> System
{
  const auto x = record[2].as<float>();
  const auto y = record[3].as<float>();
  const auto z = record[4].as<float>();

  const auto tick = record[6].as<int>();
  chrono::Duration step{
    .unit    = chrono::fromString(record[8].view()),
    .elapsed = static_cast<float>(record[7].as<int>()),
  };

  return System{
    .dbId        = core::Uuid::fromDbId(record[0].view()),
    .name        = record[1].as<std::string>(),
    .position    = Eigen::Vector3f(x, y, z),
    .currentTick = chrono::Tick::fromInt(record[5].as<int>()),
    .step        = chrono::TimeStep(tick, step),
  };
}
} // namespace

auto SystemRepository::findOneById(const core::Uuid system) const -> System
{
  const auto query = [system](pqxx::nontransaction &work) {
    return work.exec(pqxx::prepped{FIND_ONE_QUERY_NAME}, pqxx::params{system.toDbId()}).one_row();
  };
  const auto record = m_connection->executeQueryReturningSingleRow(query);

  return fromDbRow(record);
}

auto SystemRepository::findAll() const -> std::vector<System>
{
  // https://github.com/jtv/libpqxx/blob/9dd71102e1f3e10b0f14eed253873ee6ce2a4880/include/pqxx/doc/prepared-statement.md#preparing-a-statement
  const auto query = [](pqxx::nontransaction &work) {
    return work.exec(pqxx::prepped{FIND_ALL_QUERY_NAME});
  };
  const auto rows = m_connection->executeQuery(query);

  std::vector<System> out;
  for (const auto record : rows)
  {
    out.emplace_back(fromDbRow(record));
  }

  return out;
}

namespace {
auto upsertSystemQuery(pqxx::work &transaction, const System &system) -> pqxx::result
{
  transaction
    .exec(pqxx::prepped{UPDATE_SYSTEM_QUERY_NAME},
          pqxx::params{system.dbId.toDbId(),
                       system.name,
                       system.position(0),
                       system.position(1),
                       system.position(2)})
    .no_rows();

  transaction
    .exec(pqxx::prepped{UPDATE_SYSTEM_TICK_QUERY_NAME},
          pqxx::params{system.dbId.toDbId(), system.currentTick.count()})
    .no_rows();

  const auto tickData = system.step.data();

  return transaction
    .exec(pqxx::prepped{UPDATE_SYSTEM_TICK_CONFIG_QUERY_NAME},
          pqxx::params{system.dbId.toDbId(),
                       static_cast<int>(tickData.duration.elapsed),
                       str(tickData.duration.unit),
                       tickData.ticks})
    .no_rows();
}
} // namespace

void SystemRepository::save(const System &system) const
{
  auto query = [&system](pqxx::work &transaction) { return upsertSystemQuery(transaction, system); };

  const auto res = m_connection->tryExecuteTransaction(query);
  if (res.error)
  {
    error("Failed to save system: " + *res.error);
  }
}

} // namespace bsgalone::server
