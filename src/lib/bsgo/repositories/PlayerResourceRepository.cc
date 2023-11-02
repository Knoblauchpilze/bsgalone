
#include "PlayerResourceRepository.hh"

namespace bsgo {

PlayerResourceRepository::PlayerResourceRepository()
  : IRepository("player")
{
  addModule("resource");
}

auto PlayerResourceRepository::findOneById(const Uuid &resource) const -> PlayerResource
{
  PlayerResource out;
  out.resource = resource;

  switch (resource)
  {
    case Uuid{0}:
      out.name   = "tylium";
      out.amount = 1.2f;
      break;
    case Uuid{1}:
      out.name   = "titane";
      out.amount = 2.7f;
      break;
    default:
      error("Resource " + str(resource) + " not found");
      break;
  }

  return out;
}

auto PlayerResourceRepository::findAllByPlayer(const Uuid &player) const -> std::unordered_set<Uuid>
{
  if (player != Uuid(0))
  {
    error("Player " + str(player) + " not found");
  }

  return {Uuid{0}, Uuid{1}};
}

} // namespace bsgo
