
#include "PlayerShipRepository.hh"

namespace bsgo {

PlayerShipRepository::PlayerShipRepository()
  : IRepository("ship")
{}

auto PlayerShipRepository::findOneById(const Uuid &id) const -> PlayerShip
{
  if (0 != id)
  {
    error("Ship " + str(id) + "not found");
  }

  PlayerShip out;
  out.radius         = 0.5;
  out.hullPoints     = 369.2f;
  out.maxHullPoints  = 585.0f;
  out.powerPoints    = 11.9f;
  out.maxPowerPoints = 150.0f;
  out.acceleration   = 5.0f;
  out.position       = Eigen::Vector3f::Zero();

  return out;
}

} // namespace bsgo
