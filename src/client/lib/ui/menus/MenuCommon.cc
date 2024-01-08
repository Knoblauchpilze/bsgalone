
#include "MenuCommon.hh"

namespace pge {

constexpr auto DARKENING_THRESHOLD = std::uint8_t{128};

auto findHighlightColor(const Color &color) -> Color
{
  const auto hsl    = RGBToHSL(color);
  const auto factor = hsl.rgb->b > DARKENING_THRESHOLD ? 0.5f : 2.0f;

  return modulate(color, factor);
}

} // namespace pge
