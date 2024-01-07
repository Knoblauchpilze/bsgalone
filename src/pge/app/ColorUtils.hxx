
#pragma once

#include "ColorUtils.hh"
#include <maths_utils/ComparisonUtils.hh>

namespace pge {

inline olc::Pixel colorGradient(const olc::Pixel &low,
                                const olc::Pixel &high,
                                float ratio,
                                int alpha) noexcept
{
  ratio = utils::clamp(ratio, 0.0f, 1.0f);

  return olc::Pixel(static_cast<int>((1.0f - ratio) * low.r + ratio * high.r),
                    static_cast<int>((1.0f - ratio) * low.g + ratio * high.g),
                    static_cast<int>((1.0f - ratio) * low.b + ratio * high.b),
                    alpha);
}

inline olc::Pixel RGBToHSL(const olc::Pixel &rgb) noexcept
{
  // See here for more info:
  // https://stackoverflow.com/questions/47785905/converting-rgb-to-hsl-in-c
  float r = rgb.r / 255.0f;
  float g = rgb.g / 255.0f;
  float b = rgb.b / 255.0f;

  float cMin = std::min(std::min(r, g), b);
  float cMax = std::max(std::max(r, g), b);

  float delta = cMax - cMin;

  // Lightness.
  float L = (cMin + cMax) / 2.0f;

  // Saturation.
  float S = (delta == 0.0f ? 0.0f : delta / (1.0f - std::abs(2.0f * L - 1.0f)));

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

  // Hue is expressed as an angle in the range `[0; 360]`
  // and both saturation and luminance are in the range
  // `[0; 100]` so we need to convert them in the range
  // `[0; 255]`.
  int h = utils::clamp(static_cast<int>(255.0f * H / 360.0f), 0, 255);
  int s = utils::clamp(static_cast<int>(255.0f * S), 0, 255);
  int l = utils::clamp(static_cast<int>(255.0f * L), 0, 255);

  return olc::Pixel(h, s, l, rgb.a);
}

inline olc::Pixel HSLToRGB(const olc::Pixel &hsl) noexcept
{
  // See here for more info:
  // https://www.rapidtables.com/convert/color/hsl-to-rgb.html
  float h = 360.0f * hsl.r / 255.0f;
  float s = hsl.g / 255.0f;
  float l = hsl.b / 255.0f;

  float C = (1.0f - std::abs(2.0f * l - 1.0f)) * s;
  float X = C * (1.0f - std::abs(std::fmod(h / 60.0f, 2.0f) - 1.0f));

  float m = l - C / 2.0f;

  float R = 0.0f, G = 0.0f, B = 0.0f;
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

  int r = utils::clamp(static_cast<int>((R + m) * 255.0f), 0, 255);
  int g = utils::clamp(static_cast<int>((G + m) * 255.0f), 0, 255);
  int b = utils::clamp(static_cast<int>((B + m) * 255.0f), 0, 255);

  return olc::Pixel(r, g, b, hsl.a);
}

inline olc::Pixel modulate(const olc::Pixel &in, float factor) noexcept
{
  // Convert to `HSL` color space, change the lightness
  // and convert back to `RGB`. Also clamp the `factor`.
  factor = std::max(factor, 0.0f);

  olc::Pixel hsl = RGBToHSL(in);

  // We only have a tricky case: the case where `L` is
  // actually equal to `0`. In this case the `factor`
  // approach won't work, and we will rather add an
  // offset (corresponding to `1 / factor`) to still
  // make the color brighter.
  // Note that as the `hsl` values are in the range
  // `[0; 255]` we're checking against `0.01 * 255`.
  float nL = (hsl.b < 2 ? hsl.b + 255.0f / factor : hsl.b * factor);

  hsl.b = utils::clamp(static_cast<int>(nL), 0, 255);

  return HSLToRGB(hsl);
}

inline auto transparent(const olc::Pixel &in, const uint8_t alpha) -> olc::Pixel
{
  return olc::Pixel{in.r, in.g, in.b, alpha};
}

inline auto opacityFromPercentage(const float perc) -> uint8_t
{
  const auto normalized = std::min(1.0f, std::max(0.0f, perc));
  const auto opacity    = static_cast<int>(normalized * alpha::Opaque);
  return static_cast<uint8_t>(std::min(255, std::max(0, opacity)));
}

inline auto opacifyFromPercentage(const olc::Pixel &in, const float perc) -> olc::Pixel
{
  auto out = in;
  out.a    = opacityFromPercentage(perc);
  return out;
}

inline auto str(const olc::Pixel &p) noexcept -> std::string
{
  return std::string("[r: ") + std::to_string(p.r) + ", g: " + std::to_string(p.g)
         + ", b: " + std::to_string(p.b) + ", a: " + std::to_string(p.a) + "]";
}

} // namespace pge
