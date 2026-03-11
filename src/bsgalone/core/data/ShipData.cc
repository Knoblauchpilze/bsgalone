
#include "ShipData.hh"
#include "SerializationUtils.hh"

namespace bsgalone::core {

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

auto operator<<(std::ostream &out, const ShipData &data) -> std::ostream &
{
  ::core::serialize(out, data.dbId);
  ::core::serialize(out, data.faction);
  ::core::serialize(out, data.shipClass);
  ::core::serialize(out, data.name);
  ::core::serialize(out, data.maxHullPoints);
  ::core::serialize(out, data.hullPointsRegen);
  ::core::serialize(out, data.maxPowerPoints);
  ::core::serialize(out, data.powerRegen);
  ::core::serialize(out, data.acceleration);
  ::core::serialize(out, data.speed);
  ::core::serialize(out, data.radius);
  ::core::serialize(out, data.jumpTime);
  ::core::serialize(out, data.jumpTimeInThreat);
  ::core::serialize(out, data.slots);
  ::core::serialize(out, data.price);

  return out;
}

bool operator>>(std::istream &in, ShipData &data)
{
  bool ok{true};

  ok &= ::core::deserialize(in, data.dbId);
  ok &= ::core::deserialize(in, data.faction);
  ok &= ::core::deserialize(in, data.shipClass);
  ok &= ::core::deserialize(in, data.name);
  ok &= ::core::deserialize(in, data.maxHullPoints);
  ok &= ::core::deserialize(in, data.hullPointsRegen);
  ok &= ::core::deserialize(in, data.maxPowerPoints);
  ok &= ::core::deserialize(in, data.powerRegen);
  ok &= ::core::deserialize(in, data.acceleration);
  ok &= ::core::deserialize(in, data.speed);
  ok &= ::core::deserialize(in, data.radius);
  ok &= ::core::deserialize(in, data.jumpTime);
  ok &= ::core::deserialize(in, data.jumpTimeInThreat);
  ok &= ::core::deserialize(in, data.slots);
  ok &= ::core::deserialize(in, data.price);

  return ok;
}

} // namespace bsgalone::core
