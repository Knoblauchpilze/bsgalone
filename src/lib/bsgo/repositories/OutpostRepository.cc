
#include "OutpostRepository.hh"

namespace bsgo {

OutpostRepository::OutpostRepository()
  : IRepository("outpost")
{}

auto OutpostRepository::findOneById(const Uuid &outpost) const -> Outpost
{
  Outpost out;

  switch (outpost)
  {
    case 0:
      out.faction = Faction::COLONIAL;

      out.hullPoints      = 12728.0f;
      out.maxHullPoints   = 30000.0f;
      out.hullPointsRegen = 120.0f;

      out.powerPoints    = 732.0f;
      out.maxPowerPoints = 4500.0f;
      out.powerRegen     = 100.0f;

      out.radius   = 2.0f;
      out.position = Eigen::Vector3f(-6.0f, 3.2f, 0.0f);
      break;
    case 1:
      out.faction = Faction::CYLON;

      out.hullPoints      = 12728.0f;
      out.maxHullPoints   = 19000.0f;
      out.hullPointsRegen = 80.0f;

      out.powerPoints    = 152.0f;
      out.maxPowerPoints = 600.0f;
      out.powerRegen     = 12.0f;

      out.radius   = 3.0f;
      out.position = Eigen::Vector3f(6.0f, -3.2f, 0.0f);
      break;
    default:
      error("Outpost " + str(outpost) + " not found");
      break;
  }

  return out;
}

} // namespace bsgo
