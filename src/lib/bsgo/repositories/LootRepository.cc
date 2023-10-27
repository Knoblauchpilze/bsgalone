
#include "LootRepository.hh"

namespace bsgo {

LootRepository::LootRepository()
  : IRepository("loot")
{}

auto LootRepository::findOneById(const Uuid &loot) const -> Loot
{
  Loot out;

  switch (loot)
  {
    case 0:
      out.type = Item::RESOURCE;
      out.id   = Uuid(0);
      break;
    default:
      error("Loot " + str(loot) + " not found");
      break;
  }

  return out;
}

} // namespace bsgo
