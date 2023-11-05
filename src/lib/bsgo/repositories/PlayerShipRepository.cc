
#include "PlayerShipRepository.hh"

namespace bsgo {

PlayerShipRepository::PlayerShipRepository()
  : IRepository("player")
{
  addModule("ship");
}

auto PlayerShipRepository::findOneById(const Uuid &ship) const -> PlayerShip
{
  PlayerShip out;

  switch (ship)
  {
    case Uuid{0}:
      out.faction = Faction::COLONIAL;
      out.player  = {Uuid{0}};

      out.hullPoints      = 369.2f;
      out.maxHullPoints   = 585.0f;
      out.hullPointsRegen = 0.5f;

      out.powerPoints    = 11.9f;
      out.maxPowerPoints = 150.0f;
      out.powerRegen     = 2.0f;

      out.acceleration = 5.0f;
      out.speed        = 4.0f;

      out.radius   = 0.5f;
      out.position = Eigen::Vector3f::Zero();
      break;
    case Uuid{1}:
      out.faction = Faction::CYLON;

      out.hullPoints      = 280.0f;
      out.maxHullPoints   = 300.0f;
      out.hullPointsRegen = 1.0f;

      out.powerPoints    = 23.2f;
      out.maxPowerPoints = 75.0f;
      out.powerRegen     = 5.0f;

      out.acceleration = 7.0f;
      out.speed        = 6.0f;

      out.radius   = 0.7f;
      out.position = Eigen::Vector3f(2.0f, 5.0f, 0.0f);
      break;
    default:
      error("Ship for player " + str(ship) + " not found");
      break;
  }

  return out;
}

} // namespace bsgo
