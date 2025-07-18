
#include "OutpostRepository.hh"

namespace bsgo {

OutpostRepository::OutpostRepository(const DbConnectionShPtr &connection)
  : AbstractRepository("outpost", connection)
{}

namespace {
constexpr auto FIND_ONE_QUERY_NAME = "outpost_find_one";
constexpr auto FIND_ONE_QUERY
  = "SELECT faction, max_hull_points, hull_points_regen, max_power_points, power_points_regen, radius FROM outpost WHERE id = $1";
} // namespace

void OutpostRepository::initialize()
{
  m_connection->prepare(FIND_ONE_QUERY_NAME, FIND_ONE_QUERY);
}

auto OutpostRepository::findOneById(const Uuid outpost) const -> Outpost
{
  const auto query = [outpost](pqxx::nontransaction &work) {
    return work.exec(pqxx::prepped{FIND_ONE_QUERY_NAME}, pqxx::params{toDbId(outpost)}).one_row();
  };
  const auto record = m_connection->executeQueryReturningSingleRow(query);

  Outpost out;

  out.faction         = fromDbFaction(record[0].as<std::string>());
  out.maxHullPoints   = record[1].as<float>();
  out.hullPointsRegen = record[2].as<float>();
  out.maxPowerPoints  = record[3].as<float>();
  out.powerRegen      = record[4].as<float>();
  out.radius          = record[5].as<float>();

  return out;
}

} // namespace bsgo
