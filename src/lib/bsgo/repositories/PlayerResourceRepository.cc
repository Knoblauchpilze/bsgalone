
#include "PlayerResourceRepository.hh"

namespace bsgo {

PlayerResourceRepository::PlayerResourceRepository()
  : IRepository("player")
{
  addModule("resource");
}

auto PlayerResourceRepository::findOneById(const Uuid &player) const -> std::vector<Resource>
{
  if (player != Uuid(0))
  {
    error("Player " + str(player) + " not found");
  }

  std::vector<Resource> out;

  out.push_back(Resource{
    .resource = Uuid(0),
    .amount   = 1.2,
  });

  return out;
}

} // namespace bsgo
