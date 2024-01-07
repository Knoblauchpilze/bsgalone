
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

} // namespace pge