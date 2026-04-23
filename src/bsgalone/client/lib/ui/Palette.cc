
#include "Palette.hh"
#include <stdexcept>

namespace bsgalone::client {
namespace {
// https://color-register.org/color/marine-blue
constexpr pge::Color COLONIAL_BASE_COLOR{.name = {}, .rgb = pge::RgbData{1, 56, 106}};

constexpr Palette COLONIAL_PALETTE{
  .activeButtonColor   = almostOpaque(pge::colors::VERY_DARK_BLUE),
  .inactiveButtonColor = almostOpaque(pge::colors::DARK_BLUE),
  .defaultColor        = COLONIAL_BASE_COLOR,
  .almostOpaqueColor   = makeTransparent(COLONIAL_BASE_COLOR, pge::alpha::ALMOST_OPAQUE),
};

// https://color-register.org/color/marine-blue
constexpr pge::Color CYLON_BASE_COLOR{.name = {}, .rgb = pge::RgbData{170, 0, 34}};

constexpr Palette CYLON_PALETTE{
  .activeButtonColor   = almostOpaque(pge::colors::VERY_DARK_RED),
  .inactiveButtonColor = almostOpaque(pge::colors::DARK_RED),
  .defaultColor        = CYLON_BASE_COLOR,
  .almostOpaqueColor   = makeTransparent(CYLON_BASE_COLOR, pge::alpha::ALMOST_OPAQUE),
};
} // namespace

auto generatePaletteForFaction(const core::Faction faction) -> Palette
{
  switch (faction)
  {
    case core::Faction::COLONIAL:
      return COLONIAL_PALETTE;
    case core::Faction::CYLON:
      return CYLON_PALETTE;
    default:
      throw std::invalid_argument("Unsupported faction " + str(faction));
  }
}

} // namespace bsgalone::client
