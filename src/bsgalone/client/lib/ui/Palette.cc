
#include "Palette.hh"
#include <stdexcept>

namespace bsgalone::client {
namespace {
constexpr Palette COLONIAL_PALETTE{
  .activeButtonColor   = almostOpaque(pge::colors::VERY_DARK_BLUE),
  .inactiveButtonColor = almostOpaque(pge::colors::DARK_BLUE),
};

constexpr Palette CYLON_PALETTE{
  .activeButtonColor   = almostOpaque(pge::colors::VERY_DARK_RED),
  .inactiveButtonColor = almostOpaque(pge::colors::DARK_RED),
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
