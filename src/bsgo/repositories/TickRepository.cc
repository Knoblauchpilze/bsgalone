
#include "TickRepository.hh"

namespace bsgo {

TickRepository::TickRepository(const DbConnectionShPtr &connection)
  : AbstractRepository("tick", connection)
{}

namespace {
constexpr auto FIND_ONE_BY_SYSTEM_QUERY_NAME = "tick_find_one_by_system";
constexpr auto FIND_ONE_BY_SYSTEM_QUERY      = R"--(
SELECT
  tc.duration,
  tc.unit,
  tc.ticks,
  t.current_tick
FROM
  tick_config AS tc
  LEFT JOIN tick AS t ON t.system = tc.system
WHERE
  tc.system = $1
)--";
} // namespace

void TickRepository::initialize()
{
  m_connection->prepare(FIND_ONE_BY_SYSTEM_QUERY_NAME, FIND_ONE_BY_SYSTEM_QUERY);
}

auto TickRepository::findOneBySystem(const Uuid system) const -> SystemTick
{
  const auto query = [system](pqxx::nontransaction &work) {
    return work.exec(pqxx::prepped{FIND_ONE_BY_SYSTEM_QUERY_NAME}, pqxx::params{toDbId(system)})
      .one_row();
  };
  const auto record = m_connection->executeQueryReturningSingleRow(query);

  SystemTick out{};

  out.system = system;

  const auto tick = record[2].as<int>();
  chrono::Duration step{
    .unit    = chrono::fromString(record[1].view()),
    .elapsed = static_cast<float>(record[0].as<int>()),
  };

  out.step = chrono::TimeStep(tick, step);

  out.currentTick = chrono::Tick::fromInt(record[3].as<int>());

  return out;
}

} // namespace bsgo
