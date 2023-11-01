
#include "PlayerResourceRepository.hh"

namespace bsgo {

PlayerResourceRepository::PlayerResourceRepository()
  : IRepository("player")
{
  addModule("resource");
}

auto PlayerResourceRepository::findAllByPlayer(const Uuid &player) const -> std::vector<Resource>
{
  if (player != Uuid(0))
  {
    error("Player " + str(player) + " not found");
  }

  std::vector<Resource> out;

  out.push_back(Resource{
    .resource = Uuid(0),
    .name     = "tylium",
    .amount   = 1.2,
  });
  out.push_back(Resource{
    .resource = Uuid(1),
    .name     = "titane",
    .amount   = 2.7,
  });

  return out;
}

auto PlayerResourceRepository::findOneByIdAndResource(const Uuid &player,
                                                      const Uuid &resource) const -> Resource
{
  if (player != Uuid{0})
  {
    error("Player " + str(player) + " and resource " + str(resource) + " not found");
  }

  switch (resource)
  {
    case Uuid{0}:
      return Resource{
        .resource = Uuid{0},
        .name     = "tylium",
        .amount   = 1.2,
      };
    case Uuid{1}:
      return Resource{.resource = Uuid{1}, .name = "titane", .amount = 2.7};
    default:
      error("Player " + str(player) + " and resource " + str(resource) + " not found");
      break;
  }

  // Can't happen because of the error above.
  return {};
}

void PlayerResourceRepository::save(Resource resource)
{
  warn("Should save " + str(resource.resource) + " with amount " + std::to_string(resource.amount));
}

} // namespace bsgo
