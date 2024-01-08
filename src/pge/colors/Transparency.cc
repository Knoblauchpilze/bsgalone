
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

auto makeTransparentFromPercentage(const Color &in, const float perc) -> Color
{
  return Color{.name = in.name, .rgb = in.rgb, .alpha = opacityFromPercentage(perc)};
}

} // namespace pge
