
#include "OutpostRepository.hh"

namespace bsgo {

OutpostRepository::OutpostRepository()
  : IRepository("outpost")
{}

auto OutpostRepository::findOneById(const Uuid &outpost) const -> Outpost
{
  if (0 != outpost)
  {
    error("Outpost " + str(outpost) + " not found");
  }

  Outpost out;
  out.radius      = 2.0f;
  out.hullPoints  = 30000.0f;
  out.powerPoints = 4500.0f;
  out.position    = Eigen::Vector3f(-6.0f, 3.2f, 0.0f);

  return out;
}

} // namespace bsgo
