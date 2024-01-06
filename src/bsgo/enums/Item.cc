
#include "Item.hh"

namespace bsgo {

auto str(const Item item) -> std::string
{
  switch (item)
  {
    case Item::RESOURCE:
      return "resource";
    case Item::WEAPON:
      return "weapon";
    case Item::COMPUTER:
      return "computer";
    case Item::SHIP:
      return "ship";
    default:
      return "unknown";
  }
}

} // namespace bsgo
