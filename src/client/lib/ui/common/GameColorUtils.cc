
#include "GameColorUtils.hh"
#include <stdexcept>

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

auto colorFromFaction(const bsgo::Faction &faction) -> Color
{
  switch (faction)
  {
    case bsgo::Faction::COLONIAL:
      // https://color-register.org/color/marine-blue
      return Color{.rgb = RgbData{1, 56, 106}};
    case bsgo::Faction::CYLON:
      // https://www.color-hex.com/color-palette/60661
      return Color{.rgb = RgbData{170, 0, 34}};
    default:
      throw std::invalid_argument("Unsupported faction " + bsgo::str(faction));
  }
}

} // namespace pge
