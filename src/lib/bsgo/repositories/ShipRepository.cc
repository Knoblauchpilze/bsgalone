
#include "ShipRepository.hh"

namespace bsgo {

ShipRepository::ShipRepository(const DbConnectionShPtr &connection)
  : AbstractRepository("ship", connection)
{}

auto ShipRepository::findOneById(const Uuid &ship) const -> Ship
{
  Ship out;

  switch (ship)
  {
    case Uuid{0}:
      out.faction = Faction::COLONIAL;
      out.name    = "Viper mark II";

      out.maxHullPoints   = 585.0f;
      out.hullPointsRegen = 0.5f;

      out.maxPowerPoints = 150.0f;
      out.powerRegen     = 2.0f;

      out.acceleration = 5.0f;
      out.speed        = 4.0f;

      out.radius = 0.5f;
      break;
    case Uuid{1}:
      out.faction = Faction::CYLON;
      out.name    = "Cylon Raider";

      out.maxHullPoints   = 300.0f;
      out.hullPointsRegen = 1.0f;

      out.maxPowerPoints = 75.0f;
      out.powerRegen     = 5.0f;

      out.acceleration = 7.0f;
      out.speed        = 6.0f;

      out.radius = 0.7f;
      break;
    case Uuid{2}:
      out.faction = Faction::COLONIAL;
      out.name    = "Jotunn";

      out.maxHullPoints   = 3000.0f;
      out.hullPointsRegen = 50.0f;

      out.maxPowerPoints = 750.0f;
      out.powerRegen     = 32.0f;

      out.acceleration = 1.0f;
      out.speed        = 2.0f;

      out.radius = 2.0f;
      break;
    default:
      error("Ship " + str(ship) + " not found");
      break;
  }

  return out;
}

} // namespace bsgo
