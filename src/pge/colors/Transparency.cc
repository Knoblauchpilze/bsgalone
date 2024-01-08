
#include "Transparency.hh"
#include <algorithm>

namespace pge {

auto opacityFromPercentage(const float perc) -> uint8_t
{
  const auto normalized = std::clamp(perc, 0.0f, 1.0f);
  const auto opacity    = static_cast<int>(normalized * alpha::OPAQUE);

  constexpr auto MINIMUM_ALPHA = 0;
  constexpr auto MAXIMUM_ALPHA = 255;
  return static_cast<uint8_t>(std::clamp(opacity, MINIMUM_ALPHA, MAXIMUM_ALPHA));
}

auto makeTransparent(const olc::Pixel &in, const uint8_t alpha) -> olc::Pixel
{
  return olc::Pixel{in.r, in.g, in.b, alpha};
}

auto makeTransparentFromPercentage(const olc::Pixel &in, const float perc) -> olc::Pixel
{
  auto out = in;
  out.a    = opacityFromPercentage(perc);
  return out;
}

auto almostOpaque(const olc::Pixel &in) -> olc::Pixel
{
  return makeTransparent(in, alpha::ALMOST_OPAQUE);
}

auto semiOpaque(const olc::Pixel &in) -> olc::Pixel
{
  return makeTransparent(in, alpha::SEMI_OPAQUE);
}

auto almostTransparent(const olc::Pixel &in) -> olc::Pixel
{
  return makeTransparent(in, alpha::ALMOST_TRANSPARENT);
}

auto transparent(const olc::Pixel &in) -> olc::Pixel
{
  return makeTransparent(in, alpha::TRANSPARENT);
}

auto makeTransparent(const Color &in, const uint8_t alpha) -> Color
{
  return Color{.name = in.name, .rgb = in.rgb, .alpha = alpha};
}

auto makeTransparentFromPercentage(const Color &in, const float perc) -> Color
{
  return Color{.name = in.name, .rgb = in.rgb, .alpha = opacityFromPercentage(perc)};
}

auto almostOpaque(const Color &in) -> Color
{
  return makeTransparent(in, alpha::ALMOST_OPAQUE);
}

auto semiOpaque(const Color &in) -> Color
{
  return makeTransparent(in, alpha::SEMI_OPAQUE);
}

auto almostTransparent(const Color &in) -> Color
{
  return makeTransparent(in, alpha::ALMOST_TRANSPARENT);
}

auto transparent(const Color &in) -> Color
{
  return makeTransparent(in, alpha::TRANSPARENT);
}

} // namespace pge
