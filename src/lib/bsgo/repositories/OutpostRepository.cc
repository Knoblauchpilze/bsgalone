
#include "OutpostRepository.hh"

namespace bsgo {

OutpostRepository::OutpostRepository()
  : IRepository("outpost")
{}

auto OutpostRepository::findOneById(const Uuid &id) const -> Outpost
{
  if (0 != id)
  {
    error("Outpost " + str(id) + "not found");
  }

  Outpost out;
  out.radius      = 2.0f;
  out.hullPoints  = 30000.0f;
  out.powerPoints = 4500.0f;
  out.position    = Eigen::Vector3f(-6.0f, 3.2f, 0.0f);

  return out;
}

} // namespace bsgo
