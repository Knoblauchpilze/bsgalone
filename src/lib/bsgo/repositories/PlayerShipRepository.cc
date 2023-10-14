
#include "PlayerShipRepository.hh"

namespace bsgo {

PlayerShipRepository::PlayerShipRepository()
  : IRepository("ship")
{}

auto PlayerShipRepository::findOneById(const Uuid &player) const -> PlayerShip
{
  if (0 != player)
  {
    error("Ship for player " + str(player) + " not found");
  }

  PlayerShip out;
  out.faction        = Faction::COLONIAL;
  out.radius         = 0.5f;
  out.hullPoints     = 369.2f;
  out.maxHullPoints  = 585.0f;
  out.powerPoints    = 11.9f;
  out.maxPowerPoints = 150.0f;
  out.acceleration   = 5.0f;
  out.position       = Eigen::Vector3f::Zero();

  return out;
}

} // namespace bsgo
