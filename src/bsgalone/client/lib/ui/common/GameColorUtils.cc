
#include "GameColorUtils.hh"
#include <stdexcept>

namespace bsgalone::client {
constexpr auto TYLIUM_NAME = "tylium";
constexpr auto TITANE_NAME = "titane";

auto colorFromResourceName(const std::string &name) -> pge::Color
{
  if (TYLIUM_NAME == name)
  {
    return pge::colors::YELLOW;
  }
  if (TITANE_NAME == name)
  {
    return pge::colors::PURPLE;
  }

  throw std::invalid_argument("Unsupported resource name " + name);
}

auto colorFromFaction(const core::Faction faction) -> pge::Color
{
  switch (faction)
  {
    case core::Faction::COLONIAL:
      // https://color-register.org/color/marine-blue
      return pge::Color{.name = {}, .rgb = pge::RgbData{1, 56, 106}};
    case core::Faction::CYLON:
      // https://www.color-hex.com/color-palette/60661
      return pge::Color{.name = {}, .rgb = pge::RgbData{170, 0, 34}};
    default:
      throw std::invalid_argument("Unsupported faction " + core::str(faction));
  }
}

auto generatePaletteForFaction(const core::Faction faction) -> ColorPalette
{
  const auto color = colorFromFaction(faction);
  return ColorPalette{.defaultColor      = color,
                      .almostOpaqueColor = makeTransparent(color, pge::alpha::ALMOST_OPAQUE)};
}

} // namespace bsgalone::client
