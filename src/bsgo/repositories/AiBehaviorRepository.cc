
#include "AiBehaviorRepository.hh"

namespace bsgo {

AiBehaviorRepository::AiBehaviorRepository(const DbConnectionShPtr &connection)
  : AbstractRepository("ai", connection)
{
  addModule("behavior");
}

namespace {
constexpr auto FIND_ONE_BY_SHIP_QUERY_NAME = "ai_behavior_find_one_by_ship";
constexpr auto FIND_ONE_BY_SHIP_QUERY      = R"(
SELECT
  index
FROM
  ai_behaviors
WHERE
  ship = $1;
)";

constexpr auto FIND_AI_TARGETS_QUERY_NAME = "ai_behavior_find_ai_targets";
constexpr auto FIND_AI_TARGETS_QUERY      = R"(
SELECT
  x_pos,
  y_pos,
  z_pos
FROM
  ai_targets
WHERE
  ship = $1
ORDER BY
  index
)";
} // namespace

void AiBehaviorRepository::initialize()
{
  m_connection->prepare(FIND_ONE_BY_SHIP_QUERY_NAME, FIND_ONE_BY_SHIP_QUERY);
  m_connection->prepare(FIND_AI_TARGETS_QUERY_NAME, FIND_AI_TARGETS_QUERY);
}

namespace {} // namespace

auto AiBehaviorRepository::findOneByShip(const Uuid shipDbId) const -> AiBehavior
{
  const auto query = [shipDbId](pqxx::nontransaction &work) {
    return work.exec(pqxx::prepped{FIND_ONE_BY_SHIP_QUERY_NAME}, pqxx::params{toDbId(shipDbId)})
      .one_row();
  };
  const auto record = m_connection->executeQueryReturningSingleRow(query);

  AiBehavior out{.targetIndex = record[0].as<int>()};

  fetchAiTargets(shipDbId, out);

  return out;
}

// Note: this is the same as in PlayerShipRepository::fetchAiTargets
void AiBehaviorRepository::fetchAiTargets(const Uuid shipDbId, AiBehavior &out) const
{
  const auto query = [shipDbId](pqxx::nontransaction &work) {
    return work.exec(pqxx::prepped{FIND_AI_TARGETS_QUERY_NAME}, pqxx::params{toDbId(shipDbId)});
  };
  const auto rows = m_connection->executeQuery(query);

  for (const auto record : rows)
  {
    const auto x = record[0].as<float>();
    const auto y = record[1].as<float>();
    const auto z = record[2].as<float>();
    out.targets.emplace_back(x, y, z);
  }
}

} // namespace bsgo
