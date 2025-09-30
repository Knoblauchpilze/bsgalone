
#include "ShipRepository.hh"

namespace bsgo {

ShipRepository::ShipRepository(const DbConnectionShPtr &connection)
  : AbstractRepository("ship", connection)
{}

namespace {
constexpr auto FIND_ONE_QUERY_NAME = "ship_find_one";
constexpr auto FIND_ONE_QUERY      = R"(
SELECT
  s.id,
  s.faction,
  s.class,
  s.name,
  s.max_hull_points,
  s.hull_points_regen,
  s.max_power_points,
  s.power_points_regen,
  s.max_acceleration,
  s.max_speed,
  s.radius,
  sc.jump_time,
  sc.jump_time_threat
FROM
  ship AS s
  LEFT JOIN ship_class AS sc ON s.class = sc.name
WHERE
  s.id = $1
)";

constexpr auto FIND_ALL_BY_FACTION_QUERY_NAME = "ship_find_all_by_faction";
constexpr auto FIND_ALL_BY_FACTION_QUERY      = R"(
SELECT
  s.id,
  s.faction,
  s.class,
  s.name,
  s.max_hull_points,
  s.hull_points_regen,
  s.max_power_points,
  s.power_points_regen,
  s.max_acceleration,
  s.max_speed,
  s.radius,
  sc.jump_time,
  sc.jump_time_threat
FROM
  ship AS s
  LEFT JOIN ship_class AS sc ON s.class = sc.name
WHERE
  s.faction = $1
)";

constexpr auto FIND_SLOTS_QUERY_NAME = "ship_find_slots";
constexpr auto FIND_SLOTS_QUERY
  = "SELECT type, COUNT(id) FROM ship_slot WHERE ship = $1 GROUP BY type";

constexpr auto FIND_ONE_BY_STARTING_AND_FACTION_QUERY_NAME = "ship_find_starting_faction";
constexpr auto FIND_ONE_BY_STARTING_AND_FACTION_QUERY
  = "SELECT id FROM ship WHERE faction = $1 AND starting_ship = $2";
} // namespace

void ShipRepository::initialize()
{
  m_connection->prepare(FIND_ONE_QUERY_NAME, FIND_ONE_QUERY);
  m_connection->prepare(FIND_ALL_BY_FACTION_QUERY_NAME, FIND_ALL_BY_FACTION_QUERY);
  m_connection->prepare(FIND_SLOTS_QUERY_NAME, FIND_SLOTS_QUERY);
  m_connection->prepare(FIND_ONE_BY_STARTING_AND_FACTION_QUERY_NAME,
                        FIND_ONE_BY_STARTING_AND_FACTION_QUERY);
}

auto ShipRepository::findOneById(const Uuid ship) const -> Ship
{
  auto out = fetchShipBase(ship);
  fetchSlots(ship, out);

  return out;
}

namespace {
auto fetchShipFromSqlResult(const pqxx::const_result_iterator &record) -> Ship
{
  Ship ship{};
  ship.id               = fromDbId(record[0].as<int>());
  ship.faction          = fromDbFaction(record[1].as<std::string>());
  ship.shipClass        = fromDbShipClass(record[2].as<std::string>());
  ship.name             = record[3].as<std::string>();
  ship.maxHullPoints    = record[4].as<float>();
  ship.hullPointsRegen  = record[5].as<float>();
  ship.maxPowerPoints   = record[6].as<float>();
  ship.powerRegen       = record[7].as<float>();
  ship.acceleration     = record[8].as<float>();
  ship.speed            = record[9].as<float>();
  ship.radius           = record[10].as<float>();
  ship.jumpTime         = Tick::fromInt(record[11].as<int>());
  ship.jumpTimeInThreat = Tick::fromInt(record[12].as<int>());

  return ship;
}

auto fetchAllShipsByFaction(const Faction &faction, DbConnection &connection) -> std::vector<Ship>
{
  const auto query = [faction](pqxx::nontransaction &work) {
    return work.exec(pqxx::prepped{FIND_ALL_BY_FACTION_QUERY_NAME},
                     pqxx::params{toDbFaction(faction)});
  };
  const auto rows = connection.executeQuery(query);

  std::vector<Ship> out;
  for (const auto record : rows)
  {
    out.emplace_back(fetchShipFromSqlResult(record));
  }

  return out;
}
} // namespace

auto ShipRepository::findAllByFaction(const Faction &faction) const -> std::vector<Ship>
{
  auto out = fetchAllShipsByFaction(faction, *m_connection);
  for (auto &ship : out)
  {
    fetchSlots(ship.id, ship);
  }

  return out;
}

auto ShipRepository::findOneByFactionAndStarting(const Faction &faction,
                                                 const bool startingShip) const -> Ship
{
  const auto query = [faction, startingShip](pqxx::nontransaction &work) {
    return work
      .exec(pqxx::prepped{FIND_ONE_BY_STARTING_AND_FACTION_QUERY_NAME},
            pqxx::params{toDbFaction(faction), startingShip})
      .one_row();
  };
  const auto record = m_connection->executeQueryReturningSingleRow(query);

  const auto shipId = fromDbId(record[0].as<int>());

  return findOneById(shipId);
}

auto ShipRepository::fetchShipBase(const Uuid ship) const -> Ship
{
  const auto query = [ship](pqxx::nontransaction &work) {
    return work.exec(pqxx::prepped{FIND_ONE_QUERY_NAME}, pqxx::params{toDbId(ship)}).one_row();
  };
  const auto record = m_connection->executeQueryReturningSingleRow(query);

  return fetchShipFromSqlResult(record);
}

void ShipRepository::fetchSlots(const Uuid ship, Ship &out) const
{
  const auto query = [ship](pqxx::nontransaction &work) {
    return work.exec(pqxx::prepped{FIND_SLOTS_QUERY_NAME}, pqxx::params{toDbId(ship)});
  };
  const auto rows = m_connection->executeQuery(query);

  for (const auto record : rows)
  {
    const auto slot  = fromDbSlot(record[0].as<std::string>());
    const auto count = record[1].as<int>();

    out.slots[slot] = count;
  }
}

} // namespace bsgo
