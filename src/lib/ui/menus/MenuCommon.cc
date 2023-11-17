
#include "MenuCommon.hh"

namespace pge {

constexpr auto DARKENING_THRESHOLD = std::uint8_t{128};

auto findHighlightColor(const olc::Pixel &color) -> olc::Pixel
{
  const auto hsl    = RGBToHSL(color);
  const auto factor = hsl.b > DARKENING_THRESHOLD ? 0.5f : 2.0f;

  return modulate(color, factor);
}

} // namespace pge
