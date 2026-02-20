
#include "Slot.hh"
#include <stdexcept>

namespace bsgalone::core {

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

auto toDbSlot(const Slot slot) -> std::string
{
  return str(slot);
}

auto fromDbSlot(const std::string &dbSlot) -> Slot
{
  if ("weapon" == dbSlot)
  {
    return Slot::WEAPON;
  }
  if ("computer" == dbSlot)
  {
    return Slot::COMPUTER;
  }

  throw std::invalid_argument("Unknown slot \"" + dbSlot + "\"");
}

} // namespace bsgalone::core
