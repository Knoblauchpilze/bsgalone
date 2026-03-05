
#include "ShipData.hh"
#include "SerializationUtils.hh"

namespace bsgo {

bool ShipData::operator==(const ShipData &rhs) const
{
  return dbId == rhs.dbId;
}

auto ShipData::serialize(std::ostream &out) const -> std::ostream &
{
  core::serialize(out, dbId);
  core::serialize(out, faction);
  core::serialize(out, shipClass);
  core::serialize(out, name);
  core::serialize(out, maxHullPoints);
  core::serialize(out, hullPointsRegen);
  core::serialize(out, maxPowerPoints);
  core::serialize(out, powerRegen);
  core::serialize(out, acceleration);
  core::serialize(out, speed);
  core::serialize(out, radius);
  core::serialize(out, jumpTime);
  core::serialize(out, jumpTimeInThreat);

  core::serialize(out, slots);
  core::serialize(out, price);

  return out;
}

bool ShipData::deserialize(std::istream &in)
{
  bool ok{true};

  ok &= core::deserialize(in, dbId);
  ok &= core::deserialize(in, faction);
  ok &= core::deserialize(in, shipClass);
  ok &= core::deserialize(in, name);
  ok &= core::deserialize(in, maxHullPoints);
  ok &= core::deserialize(in, hullPointsRegen);
  ok &= core::deserialize(in, maxPowerPoints);
  ok &= core::deserialize(in, powerRegen);
  ok &= core::deserialize(in, acceleration);
  ok &= core::deserialize(in, speed);
  ok &= core::deserialize(in, radius);
  ok &= core::deserialize(in, jumpTime);
  ok &= core::deserialize(in, jumpTimeInThreat);

  ok &= core::deserialize(in, slots);
  ok &= core::deserialize(in, price);

  return ok;
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
