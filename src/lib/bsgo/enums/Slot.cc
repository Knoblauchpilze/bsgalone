
#include "Slot.hh"

namespace bsgo {

auto str(const Slot slot) -> std::string
{
  switch (slot)
  {
    case Slot::WEAPON:
      return "weapon";
    case Slot::COMPUTER:
      return "computer";
    default:
      return "unknown";
  }
}

} // namespace bsgo
