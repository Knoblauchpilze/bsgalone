
#include "MenuCommon.hh"
#include "ColorConversion.hh"

namespace pge {

constexpr auto DARKENING_THRESHOLD = std::uint8_t{128};

auto findHighlightColor(const olc::Pixel &color) -> olc::Pixel
{
  const auto in     = colors::toColor(color);
  const auto hsl    = RGBToHSL(in);
  const auto factor = hsl.rgb->b > DARKENING_THRESHOLD ? 0.5f : 2.0f;

  return colors::toOlcPixel(modulate(in, factor));
}

} // namespace pge
