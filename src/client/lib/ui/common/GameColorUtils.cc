
#include "GameColorUtils.hh"

namespace pge {
constexpr auto TYLIUM_NAME = "tylium";
constexpr auto TITANE_NAME = "titane";

auto colorFromResourceName(const std::string &name) -> Color
{
  if (TYLIUM_NAME == name)
  {
    return colors::YELLOW;
  }
  if (TITANE_NAME == name)
  {
    return colors::PURPLE;
  }

  throw std::invalid_argument("Unsupported resource name " + name);
}

auto colorFromFaction(const bsgo::Faction &faction) -> olc::Pixel
{
  switch (faction)
  {
    case bsgo::Faction::COLONIAL:
      return olc::DARK_BLUE;
    case bsgo::Faction::CYLON:
      return olc::DARK_RED;
    default:
      throw std::invalid_argument("Unsupported faction " + bsgo::str(faction));
  }
}

} // namespace pge
