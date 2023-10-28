
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

auto PlayerResourceRepository::findOneByIdAndResource(const Uuid &player,
                                                      const Uuid &resource) const -> Resource
{
  if (player != Uuid(0) || resource != Uuid(0))
  {
    error("Player " + str(player) + " and resource " + str(resource) + " not found");
  }

  Resource out{
    .resource = Uuid(0),
    .amount   = 1.2,
  };

  return out;
}

void PlayerResourceRepository::save(Resource resource)
{
  warn("Should save " + str(resource.resource) + " with amount " + std::to_string(resource.amount));
}

} // namespace bsgo
