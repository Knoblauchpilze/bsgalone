
#include "Item.hh"

namespace bsgo {

auto str(const Item item) -> std::string
{
  switch (item)
  {
    case Item::RESOURCE:
      return "resource";
    default:
      return "unknown";
  }
}

} // namespace bsgo
