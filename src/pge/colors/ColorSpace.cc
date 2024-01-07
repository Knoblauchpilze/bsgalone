
#include "ColorSpace.hh"
#include <algorithm>

namespace pge {

auto colorGradient(const olc::Pixel &low, const olc::Pixel &high, float ratio, int alpha) noexcept
  -> olc::Pixel
{
  ratio = std::clamp(ratio, 0.0f, 1.0f);
  return olc::Pixel(static_cast<int>((1.0f - ratio) * low.r + ratio * high.r),
                    static_cast<int>((1.0f - ratio) * low.g + ratio * high.g),
                    static_cast<int>((1.0f - ratio) * low.b + ratio * high.b),
                    alpha);
}

auto RGBToHSL(const olc::Pixel &rgb) noexcept -> olc::Pixel
{
  // See here for more info:
  // https://stackoverflow.com/questions/47785905/converting-rgb-to-hsl-in-c
  const auto r = rgb.r / 255.0f;
  const auto g = rgb.g / 255.0f;
  const auto b = rgb.b / 255.0f;

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

  return olc::Pixel(h, s, l, rgb.a);
}

auto HSLToRGB(const olc::Pixel &hsl) noexcept -> olc::Pixel
{
  // See here for more info:
  // https://www.rapidtables.com/convert/color/hsl-to-rgb.html
  const auto h = 360.0f * hsl.r / 255.0f;
  const auto s = hsl.g / 255.0f;
  const auto l = hsl.b / 255.0f;

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

  return olc::Pixel(r, g, b, hsl.a);
}

auto modulate(const olc::Pixel &in, float factor) noexcept -> olc::Pixel
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
  const auto nL = (hsl.b < LUMINANCE_THRESHOLD_TO_USE_OFFSET_APPROACH ? hsl.b + 255.0f / factor
                                                                      : hsl.b * factor);

  hsl.b = static_cast<uint8_t>(std::clamp(static_cast<int>(nL), 0, 255));

  return HSLToRGB(hsl);
}

} // namespace pge
