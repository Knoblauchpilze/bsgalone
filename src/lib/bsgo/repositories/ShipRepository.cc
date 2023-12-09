
#include "ShipRepository.hh"

namespace bsgo {

ShipRepository::ShipRepository(const DbConnectionShPtr &connection)
  : AbstractRepository("ship", connection)
{}

namespace {
constexpr auto FIND_ONE_QUERY_NAME = "ship_find_one";
constexpr auto FIND_ONE_QUERY
  = "SELECT s.faction, sc.class, s.name, s.max_hull_points, s.hull_points_regen, s.max_power_points, s.power_points_regen, s.max_acceleration, s.max_speed, s.radius sc.jump_time_ms, sc.jump_time_threat_ms FROM ship AS s LEFT JOIN ship_class AS sc ON s.class = sc.name WHERE s.id = $1";

constexpr auto FIND_SLOTS_QUERY_NAME = "ship_find_slots";
constexpr auto FIND_SLOTS_QUERY
  = "SELECT type, COUNT(id) FROM ship_slot WHERE ship = $1 GROUP BY type";
} // namespace

void ShipRepository::initialize()
{
  m_connection->prepare(FIND_ONE_QUERY_NAME, FIND_ONE_QUERY);
  m_connection->prepare(FIND_SLOTS_QUERY_NAME, FIND_SLOTS_QUERY);
}

auto ShipRepository::findOneById(const Uuid &ship) const -> Ship
{
  auto out = fetchShipBase(ship);
  fetchSlots(ship, out);

  return out;
}

auto ShipRepository::fetchShipBase(const Uuid &ship) const -> Ship
{
  auto work         = m_connection->nonTransaction();
  const auto record = work.exec_prepared1(FIND_ONE_QUERY_NAME, toDbId(ship));

  Ship out;

  out.faction          = fromDbFaction(record[0].as<std::string>());
  out.shipClass        = fromDbShipClass(record[1].as<std::string>());
  out.name             = record[2].as<std::string>();
  out.maxHullPoints    = record[3].as<float>();
  out.hullPointsRegen  = record[4].as<float>();
  out.maxPowerPoints   = record[5].as<float>();
  out.powerRegen       = record[6].as<float>();
  out.acceleration     = record[7].as<float>();
  out.speed            = record[8].as<float>();
  out.radius           = record[9].as<float>();
  out.jumpTime         = utils::Milliseconds(record[10].as<int>());
  out.jumpTimeInThreat = utils::Milliseconds(record[11].as<int>());

  return out;
}

void ShipRepository::fetchSlots(const Uuid &ship, Ship &out) const
{
  auto work       = m_connection->nonTransaction();
  const auto rows = work.exec_prepared(FIND_SLOTS_QUERY_NAME, toDbId(ship));

  for (const auto record : rows)
  {
    const auto slot  = fromDbSlot(record[0].as<std::string>());
    const auto count = record[1].as<int>();

    out.slots[slot] = count;
  }
}

} // namespace bsgo
