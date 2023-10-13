
#include "ShipRepository.hh"

namespace bsgo {

ShipRepository::ShipRepository()
  : IRepository("ship")
{}

auto ShipRepository::findOneById(const Uuid &id) const -> Ship
{
  if (0 != id)
  {
    error("Ship " + str(id) + "not found");
  }

  Ship out;
  out.radius       = 0.5f;
  out.hullPoints   = 585.0f;
  out.powerPoints  = 150.0f;
  out.acceleration = 5.0f;

  return out;
}

auto ShipRepository::findPositionById(const Uuid &id) const -> Eigen::Vector3f
{
  if (0 != id)
  {
    error("Ship " + str(id) + "not found");
  }

  return Eigen::Vector3f::Zero();
}

} // namespace bsgo
