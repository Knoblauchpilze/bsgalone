
#include "Color.hh"
#include "ColorConversion.hh"

namespace pge {

auto toRgb(const Color &color) -> Color
{
  if (color.rgb)
  {
    return color;
  }

  if (!color.name)
  {
    throw std::invalid_argument("Invalid color with no rgb data and no name");
  }

  auto pixel = toOlcPixel(*color.name);

  const RgbData rgb = {.r = pixel.r, .g = pixel.g, .b = pixel.b};
  return Color{.rgb = rgb, .alpha = color.alpha};
}

} // namespace pge
