
#include "ShipData.hh"

namespace bsgo {

bool ShipData::operator==(const ShipData &rhs) const
{
  return dbId == rhs.dbId;
}

auto fromDbShip(const Ship &ship, const ShipPriceRepository &repository) -> ShipData
{
  ShipData out{
    .dbId            = ship.id,
    .faction         = ship.faction,
    .shipClass       = ship.shipClass,
    .name            = ship.name,
    .maxHullPoints   = ship.maxHullPoints,
    .hullPointsRegen = ship.hullPointsRegen,
    .maxPowerPoints  = ship.maxPowerPoints,
    .powerRegen      = ship.powerRegen,
    .acceleration    = ship.acceleration,
    .speed           = ship.speed,
    .radius          = ship.radius,

    .jumpTime         = ship.jumpTime,
    .jumpTimeInThreat = ship.jumpTimeInThreat,

    .slots = ship.slots,
  };

  for (const auto &[resource, cost] : repository.findAllByShip(ship.id))
  {
    out.price[resource] = cost;
  }

  return out;
}

} // namespace bsgo
