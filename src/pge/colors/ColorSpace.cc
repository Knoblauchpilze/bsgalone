
#include "ColorSpace.hh"
#include <algorithm>
#include <cmath>

namespace pge {

auto colorGradient(const Color &low, const Color &high, float ratio, const uint8_t alpha) noexcept
  -> Color
{
  ratio              = std::clamp(ratio, 0.0f, 1.0f);
  const auto rgbLow  = toRgb(low);
  const auto rgbHigh = toRgb(high);

  RgbData out{.r = static_cast<uint8_t>((1.0f - ratio) * rgbLow.rgb->r + ratio * rgbHigh.rgb->r),
              .g = static_cast<uint8_t>((1.0f - ratio) * rgbLow.rgb->g + ratio * rgbHigh.rgb->g),
              .b = static_cast<uint8_t>((1.0f - ratio) * rgbLow.rgb->b + ratio * rgbHigh.rgb->b)};

  return Color{.name = {}, .rgb = out, .alpha = alpha};
}

auto RGBToHSL(const Color &color) noexcept -> Color
{
  // See here for more info:
  // https://stackoverflow.com/questions/47785905/converting-rgb-to-hsl-in-c
  const auto in = toRgb(color);
  const auto r  = in.rgb->r / 255.0f;
  const auto g  = in.rgb->g / 255.0f;
  const auto b  = in.rgb->b / 255.0f;

  const auto cMin = std::min(std::min(r, g), b);
  const auto cMax = std::max(std::max(r, g), b);

  const auto delta = cMax - cMin;

  // Lightness.
  const auto L = (cMin + cMax) / 2.0f;

  // Saturation.
  const auto S = (delta == 0.0f ? 0.0f : delta / (1.0f - std::abs(2.0f * L - 1.0f)));

  // Hue.
  float H = 0.0f;
  if (cMin >= cMax)
  {
    H = 0.0f;
  }
  else if (r >= cMax)
  {
    H = 60.0f * std::fmod((g - b) / delta, 6.0f);
  }
  else if (g >= cMax)
  {
    H = 60.0f * ((b - r) / delta + 2.0f);
  }
  else if (b >= cMax)
  {
    H = 60.0f * ((r - g) / delta + 4.0f);
  }

  if (H < 0.0f)
  {
    H = H + 360.0f;
  }

  // Hue is expressed as an angle in the range `[0; 360]` and both
  // saturation and luminance are in the range `[0; 100]` so we need
  // to convert them in the range `[0; 255]`.
  const auto h = static_cast<uint8_t>(std::clamp(static_cast<int>(255.0f * H / 360.0f), 0, 255));
  const auto s = static_cast<uint8_t>(std::clamp(static_cast<int>(255.0f * S), 0, 255));
  const auto l = static_cast<uint8_t>(std::clamp(static_cast<int>(255.0f * L), 0, 255));

  RgbData out{.r = h, .g = s, .b = l};
  return Color{.name = {}, .rgb = out, .alpha = in.alpha};
}

auto HSLToRGB(const Color &color) noexcept -> Color
{
  // See here for more info:
  // https://www.rapidtables.com/convert/color/hsl-to-rgb.html
  const auto in = toRgb(color);
  const auto h  = 360.0f * in.rgb->r / 255.0f;
  const auto s  = in.rgb->g / 255.0f;
  const auto l  = in.rgb->b / 255.0f;

  const auto C = (1.0f - std::abs(2.0f * l - 1.0f)) * s;
  const auto X = C * (1.0f - std::abs(std::fmod(h / 60.0f, 2.0f) - 1.0f));

  const auto m = l - C / 2.0f;

  auto R = 0.0f, G = 0.0f, B = 0.0f;
  if (h < 60.0f)
  {
    R = C;
    G = X;
    B = 0.0f;
  }
  else if (h < 120.0f)
  {
    R = X;
    G = C;
    B = 0.0f;
  }
  else if (h < 180.0f)
  {
    R = 0.0f;
    G = C;
    B = X;
  }
  else if (h < 240.0f)
  {
    R = 0.0f;
    G = X;
    B = C;
  }
  else if (h < 300.0f)
  {
    R = X;
    G = 0.0f;
    B = C;
  }
  else
  {
    R = C;
    G = 0.0f;
    B = X;
  }

  const auto r = static_cast<uint8_t>(std::clamp(static_cast<int>((R + m) * 255.0f), 0, 255));
  const auto g = static_cast<uint8_t>(std::clamp(static_cast<int>((G + m) * 255.0f), 0, 255));
  const auto b = static_cast<uint8_t>(std::clamp(static_cast<int>((B + m) * 255.0f), 0, 255));

  RgbData out{.r = r, .g = g, .b = b};
  return Color{.name = {}, .rgb = out, .alpha = in.alpha};
}

auto modulate(const Color &in, float factor) noexcept -> Color
{
  // Convert to `HSL` color space, change the lightness
  // and convert back to `RGB`. Also clamp the `factor`.
  factor = std::max(factor, 0.0f);

  auto hsl = RGBToHSL(in);

  // We only have a tricky case: the case where `L` is
  // actually equal to `0`. In this case the `factor`
  // approach won't work, and we will rather add an
  // offset (corresponding to `1 / factor`) to still
  // make the color brighter.
  // Note that as the `hsl` values are in the range
  // `[0; 255]` we're checking against `0.01 * 255`.
  constexpr auto LUMINANCE_THRESHOLD_TO_USE_OFFSET_APPROACH = 2;
  const auto nL = (hsl.rgb->b < LUMINANCE_THRESHOLD_TO_USE_OFFSET_APPROACH
                     ? hsl.rgb->b + 255.0f / factor
                     : hsl.rgb->b * factor);

  hsl.rgb->b = static_cast<uint8_t>(std::clamp(static_cast<int>(nL), 0, 255));

  return HSLToRGB(hsl);
}

} // namespace pge
