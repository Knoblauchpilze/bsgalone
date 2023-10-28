
#include "AsteroidLootRepository.hh"

namespace bsgo {

AsteroidLootRepository::AsteroidLootRepository()
  : IRepository("loot")
{
  addModule("asteroid");
}

auto AsteroidLootRepository::findOneById(const Uuid &loot) const -> AsteroidLoot
{
  AsteroidLoot out;

  switch (loot)
  {
    case 0:
      out.resource = Uuid(0);
      out.amount   = 10;
      break;
    default:
      error("Asteroid loot " + str(loot) + " not found");
      break;
  }

  return out;
}

} // namespace bsgo
