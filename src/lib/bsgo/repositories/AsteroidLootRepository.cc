
#include "AsteroidLootRepository.hh"

namespace bsgo {

AsteroidLootRepository::AsteroidLootRepository()
  : IRepository("loot")
{
  addModule("asteroid");
}

auto AsteroidLootRepository::findOneById(const Uuid &asteroid) const -> AsteroidLoot
{
  AsteroidLoot out;

  switch (asteroid)
  {
    case 0:
      out.resource = Uuid(0);
      out.amount   = 10;
      break;
    case 2:
      out.resource = Uuid(0);
      out.amount   = 2;
      break;
    default:
      error("Loot for asteroid " + str(asteroid) + " not found");
      break;
  }

  return out;
}

} // namespace bsgo
