
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
  return Color{.name = color.name, .rgb = rgb, .alpha = color.alpha};
}

auto str(const Color &color) -> std::string
{
  if (color.name)
  {
    return str(*color.name);
  }

  if (!color.rgb)
  {
    return "[no-data]";
  }

  std::string out{"["};

  out += "r: ";
  out += std::to_string(color.rgb->r);
  out += ", ";

  out += "g: ";
  out += std::to_string(color.rgb->g);
  out += ", ";

  out += "b: ";
  out += std::to_string(color.rgb->b);
  out += ", ";

  out += "a: ";
  out += std::to_string(color.alpha);

  out += "]";

  return out;
}

} // namespace pge
