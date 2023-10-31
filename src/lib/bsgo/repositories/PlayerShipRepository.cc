
#include "PlayerShipRepository.hh"

namespace bsgo {

PlayerShipRepository::PlayerShipRepository()
  : IRepository("player")
{
  addModule("ship");
}

auto PlayerShipRepository::findOneById(const Uuid &player) const -> PlayerShip
{
  if (0 != player)
  {
    error("Ship for player " + str(player) + " not found");
  }

  PlayerShip out;
  out.faction = Faction::COLONIAL;
  out.player  = {player};

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

  out.weapons   = {Uuid(0), Uuid(1)};
  out.computers = {Uuid(0), Uuid(1)};

  return out;
}

} // namespace bsgo
